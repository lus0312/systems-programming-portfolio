#!/usr/bin/env python3
"""Integration test runner for a local ft_irc server.

The script uses only Python's standard library and opens real TCP clients.
It can connect to an already running server or start ./ircserv itself.
"""

from __future__ import annotations

import argparse
import os
import re
import select
import signal
import shlex
import socket
import subprocess
import sys
import tempfile
import time
from datetime import datetime
from typing import Callable, Dict, List, Optional, Sequence, Set, Tuple


class TestFailure(AssertionError):
    pass


class IRCMessage:
    def __init__(self, raw: str, prefix: Optional[str], command: str, params: List[str]):
        self.raw = raw
        self.prefix = prefix
        self.command = command
        self.params = params


def parse_irc_line(line: str) -> IRCMessage:
    """Parse enough IRC syntax for assertions used by this test runner."""
    rest = line.strip("\r\n")
    prefix: Optional[str] = None

    if rest.startswith(":"):
        if " " not in rest:
            return IRCMessage(line, rest[1:], "", [])
        prefix, rest = rest[1:].split(" ", 1)

    trailing: Optional[str] = None
    if " :" in rest:
        rest, trailing = rest.split(" :", 1)

    parts = rest.split()
    if not parts:
        return IRCMessage(line, prefix, "", [])

    command = parts[0].upper()
    params = parts[1:]
    if trailing is not None:
        params.append(trailing)
    return IRCMessage(line, prefix, command, params)


def prefix_nick(prefix: Optional[str]) -> str:
    if not prefix:
        return ""
    return prefix.split("!", 1)[0].split("@", 1)[0]


def base36(value: int) -> str:
    alphabet = "0123456789abcdefghijklmnopqrstuvwxyz"
    if value == 0:
        return "0"
    result = ""
    while value:
        value, remainder = divmod(value, 36)
        result = alphabet[remainder] + result
    return result



class Style:
    COLORS = {
        "reset": "\033[0m",
        "bold": "\033[1m",
        "dim": "\033[2m",
        "red": "\033[31m",
        "green": "\033[32m",
        "yellow": "\033[33m",
        "blue": "\033[34m",
        "magenta": "\033[35m",
        "cyan": "\033[36m",
        "gray": "\033[90m",
    }

    def __init__(self, enabled: bool) -> None:
        self.enabled = enabled

    def apply(self, text: str, *names: str) -> str:
        if not self.enabled:
            return text
        codes = "".join(self.COLORS[name] for name in names if name in self.COLORS)
        if not codes:
            return text
        return codes + text + self.COLORS["reset"]


class TraceWriter:
    def __init__(self, path: str) -> None:
        self.path = path
        self.started_at = time.monotonic()
        directory = os.path.dirname(path)
        if directory:
            os.makedirs(directory, exist_ok=True)
        self._handle = open(path, "w", encoding="utf-8")

    def close(self) -> None:
        self._handle.close()

    def elapsed(self) -> str:
        return f"{time.monotonic() - self.started_at:8.3f}s"

    def line(self, text: str = "") -> None:
        self._handle.write(text + "\n")
        self._handle.flush()

    def event(self, text: str) -> None:
        self.line(f"{self.elapsed()} | {text}")

    def client_line(self, label: str, direction: str, text: str) -> None:
        arrow = "C->S" if direction == ">>" else "S->C"
        self.line(f"{self.elapsed()} | {label:<12} | {arrow:<4} | {text}")

    def header(self, args: argparse.Namespace, selected_names: Sequence[str]) -> None:
        self.line("# ft_irc tester trace")
        self.line(f"created_at: {datetime.now().isoformat(timespec='seconds')}")
        self.line("command: " + " ".join(shlex.quote(arg) for arg in sys.argv))
        self.line(f"target: {args.host}:{args.port}")
        if args.spawn:
            self.line(f"spawn: {args.spawn}")
        self.line("selected_tests: " + ", ".join(selected_names))
        self.line("")

    def test_start(self, index: int, total: int, name: str) -> None:
        self.line("")
        self.line("=" * 78)
        self.line(f"TEST {index}/{total}: {name}")
        self.line("=" * 78)

    def test_result(self, status: str, name: str, elapsed: float, detail: str = "") -> None:
        self.line(f"{self.elapsed()} | RESULT | {status} | {name} | {elapsed:.2f}s")
        if detail:
            self.line(f"{self.elapsed()} | DETAIL | {detail}")

class IRCClient:
    def __init__(
        self,
        host: str,
        port: int,
        label: str,
        timeout: float,
        print_raw: bool,
        trace: Optional[TraceWriter],
    ) -> None:
        self.host = host
        self.port = port
        self.label = label
        self.timeout = timeout
        self.print_raw = print_raw
        self.trace = trace
        self.nick = ""
        self._buffer = b""
        self.pending: List[str] = []
        self.history: List[str] = []
        self.closed = False

        try:
            self.sock = socket.create_connection((host, port), timeout=timeout)
        except OSError as exc:
            raise TestFailure(f"{label}: cannot connect to {host}:{port}: {exc}")
        self.sock.setblocking(False)
        if self.trace is not None:
            self.trace.event(f"CONNECT | {self.label} | {host}:{port}")

    def _log(self, direction: str, text: str) -> None:
        entry = f"{direction} {text}"
        self.history.append(entry)
        if self.trace is not None:
            self.trace.client_line(self.label, direction, text)
        if self.print_raw:
            print(f"    [{self.label}] {entry}")

    def send_line(self, line: str) -> None:
        if "\r" in line or "\n" in line:
            raise ValueError("send_line() expects one IRC line without CR/LF")
        self.send_raw((line + "\r\n").encode("utf-8"))
        self._log(">>", line)

    def send_raw(self, payload: bytes) -> None:
        if self.closed:
            raise TestFailure(f"{self.label}: attempted to write to a closed socket")

        view = memoryview(payload)
        deadline = time.monotonic() + self.timeout
        while view:
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                raise TestFailure(f"{self.label}: timed out while sending data")
            try:
                _, writable, exceptional = select.select([], [self.sock], [self.sock], remaining)
            except (OSError, ValueError) as exc:
                raise TestFailure(f"{self.label}: select() failed while sending: {exc}")
            if exceptional:
                raise TestFailure(f"{self.label}: socket error while sending")
            if not writable:
                continue
            try:
                sent = self.sock.send(view)
            except BlockingIOError:
                continue
            except OSError as exc:
                raise TestFailure(f"{self.label}: send() failed: {exc}")
            if sent == 0:
                raise TestFailure(f"{self.label}: socket closed while sending")
            view = view[sent:]

    def _pump(self, wait: float) -> int:
        if self.closed:
            return 0
        try:
            readable, _, exceptional = select.select([self.sock], [], [self.sock], max(0.0, wait))
        except (OSError, ValueError):
            self.closed = True
            return 0
        if exceptional:
            self.closed = True
            return 0
        if not readable:
            return 0

        received = 0
        while True:
            try:
                chunk = self.sock.recv(4096)
            except BlockingIOError:
                break
            except OSError:
                self.closed = True
                break
            if not chunk:
                self.closed = True
                break
            received += len(chunk)
            self._buffer += chunk

        while b"\r\n" in self._buffer:
            raw, self._buffer = self._buffer.split(b"\r\n", 1)
            line = raw.decode("utf-8", errors="replace")
            self.pending.append(line)
            self._log("<<", line)
        return received

    def expect(
        self,
        predicate: Callable[[str], bool],
        description: str,
        timeout: Optional[float] = None,
    ) -> str:
        deadline = time.monotonic() + (self.timeout if timeout is None else timeout)

        while True:
            for index, line in enumerate(self.pending):
                if predicate(line):
                    del self.pending[index]
                    if self.trace is not None:
                        self.trace.event(f"EXPECT OK | {self.label} | {description}")
                    return line

            remaining = deadline - time.monotonic()
            if remaining <= 0:
                break
            self._pump(min(0.05, remaining))
            if self.closed and not self.pending:
                break

        tail = "\n".join(f"      {item}" for item in self.history[-12:])
        if not tail:
            tail = "      (no traffic recorded)"
        if self.trace is not None:
            self.trace.event(f"EXPECT FAIL | {self.label} | {description}")
        raise TestFailure(
            f"{self.label}: expected {description}, but it was not received.\n"
            f"    Recent traffic:\n{tail}"
        )

    def collect(self, duration: float) -> List[str]:
        deadline = time.monotonic() + duration
        while True:
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                break
            self._pump(min(0.05, remaining))
            if self.closed:
                break
        lines = list(self.pending)
        self.pending.clear()
        return lines

    def drain(self, duration: float = 0.08) -> None:
        self.collect(duration)

    def close(self) -> None:
        if getattr(self, "sock", None) is None:
            return
        try:
            self.sock.close()
        except OSError:
            pass
        self.closed = True
        if self.trace is not None:
            self.trace.event(f"CLOSE | {self.label}")


def message_matches(
    line: str,
    command: str,
    params: Optional[Sequence[str]] = None,
    nick: Optional[str] = None,
    contains: Optional[Sequence[str]] = None,
) -> bool:
    message = parse_irc_line(line)
    if message.command != command.upper():
        return False
    if nick is not None and prefix_nick(message.prefix) != nick:
        return False
    if params is not None:
        if len(message.params) < len(params):
            return False
        for actual, expected in zip(message.params, params):
            if actual != expected:
                return False
    if contains is not None:
        for fragment in contains:
            if fragment not in line:
                return False
    return True


def expect_numeric(
    client: IRCClient,
    code: int,
    extra: Optional[Callable[[IRCMessage], bool]] = None,
) -> IRCMessage:
    command = f"{code:03d}"

    def predicate(line: str) -> bool:
        message = parse_irc_line(line)
        return message.command == command and (extra is None or extra(message))

    line = client.expect(predicate, f"numeric {command}")
    return parse_irc_line(line)


def expect_command(
    client: IRCClient,
    command: str,
    check: Optional[Callable[[IRCMessage], bool]] = None,
    description: Optional[str] = None,
) -> IRCMessage:
    command = command.upper()

    def predicate(line: str) -> bool:
        message = parse_irc_line(line)
        return message.command == command and (check is None or check(message))

    line = client.expect(predicate, description or command)
    return parse_irc_line(line)


def expect_join(client: IRCClient, nick: str, channel: str) -> IRCMessage:
    return expect_command(
        client,
        "JOIN",
        lambda message: prefix_nick(message.prefix) == nick
        and bool(message.params)
        and message.params[0] == channel,
        f"JOIN for {nick} on {channel}",
    )


def expect_mode(client: IRCClient, channel: str, modes: str) -> IRCMessage:
    return expect_command(
        client,
        "MODE",
        lambda message: len(message.params) >= 2
        and message.params[0] == channel
        and message.params[1] == modes,
        f"MODE {channel} {modes}",
    )


def expect_privmsg(client: IRCClient, sender: str, target: str, text: str) -> IRCMessage:
    return expect_command(
        client,
        "PRIVMSG",
        lambda message: prefix_nick(message.prefix) == sender
        and len(message.params) >= 2
        and message.params[0] == target
        and message.params[1] == text,
        f"PRIVMSG from {sender} to {target}: {text}",
    )


def expect_topic(client: IRCClient, sender: str, channel: str, topic: str) -> IRCMessage:
    return expect_command(
        client,
        "TOPIC",
        lambda message: prefix_nick(message.prefix) == sender
        and len(message.params) >= 2
        and message.params[0] == channel
        and message.params[1] == topic,
        f"TOPIC {channel} :{topic}",
    )


def expect_kick(client: IRCClient, sender: str, channel: str, target: str) -> IRCMessage:
    return expect_command(
        client,
        "KICK",
        lambda message: prefix_nick(message.prefix) == sender
        and len(message.params) >= 2
        and message.params[0] == channel
        and message.params[1] == target,
        f"KICK {channel} {target}",
    )


def expect_part(client: IRCClient, sender: str, channel: str) -> IRCMessage:
    return expect_command(
        client,
        "PART",
        lambda message: prefix_nick(message.prefix) == sender
        and bool(message.params)
        and message.params[0] == channel,
        f"PART {channel} from {sender}",
    )


class TestContext:
    def __init__(
        self,
        args: argparse.Namespace,
        test_name: str,
        test_index: int,
        trace: Optional[TraceWriter],
    ) -> None:
        self.args = args
        self.test_name = test_name
        self.test_index = test_index
        self.trace = trace
        self.clients: List[IRCClient] = []
        self._counter = 0
        salt = (int(time.time() * 1000) ^ os.getpid() ^ (test_index << 8)) % (36 ** 5)
        self.tag = base36(salt).rjust(5, "0")

    def unique_nick(self, prefix: str = "u") -> str:
        self._counter += 1
        clean = re.sub(r"[^A-Za-z0-9]", "", prefix) or "u"
        # Starts with a letter and stays short enough for conservative IRC clients.
        return (clean[0].upper() + self.tag + base36(self._counter))[:15]

    def channel(self, suffix: str) -> str:
        clean = re.sub(r"[^A-Za-z0-9]", "", suffix) or "x"
        return f"#T{self.tag}{clean}"[:40]

    def connect(self, label: str) -> IRCClient:
        client = IRCClient(
            self.args.host,
            self.args.port,
            label,
            self.args.timeout,
            self.args.print_raw,
            self.trace,
        )
        self.clients.append(client)
        return client

    def register(self, label: str, nick_prefix: str = "u") -> IRCClient:
        client = self.connect(label)
        nick = self.unique_nick(nick_prefix)
        username = ("usr" + self.tag + base36(self._counter))[:16]
        client.send_line(f"PASS {self.args.password}")
        client.send_line(f"NICK {nick}")
        client.send_line(f"USER {username} 0 * :ft_irc automated test")
        expect_numeric(client, 1, lambda message: nick in message.params)
        client.nick = nick
        client.drain()
        return client

    def join(self, client: IRCClient, channel: str, key: Optional[str] = None) -> None:
        command = f"JOIN {channel}"
        if key is not None:
            command += f" {key}"
        client.send_line(command)
        expect_join(client, client.nick, channel)
        expect_numeric(client, 353, lambda message: channel in message.params)
        expect_numeric(client, 366, lambda message: channel in message.params)

    def close(self) -> None:
        for client in self.clients:
            client.close()
        # Give the server a brief chance to process the peer closes before the next case.
        time.sleep(0.03)

    def transcript(self) -> str:
        blocks: List[str] = []
        for client in self.clients:
            if not client.history:
                continue
            lines = "\n".join(f"      {line}" for line in client.history[-16:])
            blocks.append(f"    [{client.label}]\n{lines}")
        return "\n".join(blocks)


def query_modes(client: IRCClient, channel: str) -> Tuple[str, List[str]]:
    client.send_line(f"MODE {channel}")
    response = expect_numeric(
        client,
        324,
        lambda message: len(message.params) >= 3 and message.params[1] == channel,
    )
    return response.params[2], response.params[3:]


def assert_modes(
    client: IRCClient,
    channel: str,
    present: str = "",
    absent: str = "",
    args_present: Sequence[str] = (),
) -> None:
    modes, mode_args = query_modes(client, channel)
    flags = set(character for character in modes if character.isalpha())
    for flag in present:
        if flag not in flags:
            raise TestFailure(
                f"{client.label}: MODE query for {channel} returned {modes!r}; "
                f"expected flag {flag!r}"
            )
    for flag in absent:
        if flag in flags:
            raise TestFailure(
                f"{client.label}: MODE query for {channel} returned {modes!r}; "
                f"flag {flag!r} should be absent"
            )
    for value in args_present:
        if value not in mode_args:
            raise TestFailure(
                f"{client.label}: MODE query for {channel} returned args {mode_args!r}; "
                f"expected {value!r}"
            )


def test_registration(ctx: TestContext) -> None:
    pre = ctx.connect("pre-reg")
    pre.send_line(f"JOIN {ctx.channel('pre')}")
    expect_numeric(pre, 451)

    pre.send_line("PASS definitely-wrong")
    expect_numeric(pre, 464)

    first_nick = ctx.unique_nick("a")
    pre.send_line(f"PASS {ctx.args.password}")
    pre.send_line(f"NICK {first_nick}")
    pre.send_line(f"USER u{ctx.tag} 0 * :first client")
    expect_numeric(pre, 1, lambda message: first_nick in message.params)
    pre.nick = first_nick

    pre.send_line(f"PASS {ctx.args.password}")
    expect_numeric(pre, 462)

    pre.send_line("PING :automation-probe")
    expect_command(
        pre,
        "PONG",
        lambda message: "automation-probe" in message.params,
        "PONG for automation-probe",
    )

    duplicate = ctx.connect("duplicate")
    duplicate.send_line(f"PASS {ctx.args.password}")
    duplicate.send_line(f"USER dup{ctx.tag} 0 * :duplicate test")
    duplicate.send_line(f"NICK {first_nick}")
    expect_numeric(duplicate, 433)

    replacement = ctx.unique_nick("b")
    duplicate.send_line(f"NICK {replacement}")
    expect_numeric(duplicate, 1, lambda message: replacement in message.params)
    duplicate.nick = replacement


def test_join_and_messaging(ctx: TestContext) -> None:
    alice = ctx.register("alice", "a")
    bob = ctx.register("bob", "b")
    channel = ctx.channel("msg")

    ctx.join(alice, channel)
    alice.drain()
    ctx.join(bob, channel)
    expect_join(alice, bob.nick, channel)
    alice.drain()
    bob.drain()

    channel_text = "hello channel"
    alice.send_line(f"PRIVMSG {channel} :{channel_text}")
    expect_privmsg(bob, alice.nick, channel, channel_text)
    sender_lines = alice.collect(ctx.args.settle)
    if any(
        message_matches(line, "PRIVMSG", params=[channel, channel_text], nick=alice.nick)
        for line in sender_lines
    ):
        raise TestFailure("channel PRIVMSG was echoed back to the sender")

    direct_text = "private hello"
    alice.send_line(f"PRIVMSG {bob.nick} :{direct_text}")
    expect_privmsg(bob, alice.nick, bob.nick, direct_text)

    bob.send_line(f"PART {channel} :done")
    expect_part(bob, bob.nick, channel)
    expect_part(alice, bob.nick, channel)


def test_topic_permissions(ctx: TestContext) -> None:
    operator = ctx.register("operator", "o")
    regular = ctx.register("regular", "r")
    channel = ctx.channel("topic")

    ctx.join(operator, channel)
    operator.drain()
    ctx.join(regular, channel)
    expect_join(operator, regular.nick, channel)
    operator.drain()
    regular.drain()

    operator.send_line(f"MODE {channel} +t")
    expect_mode(operator, channel, "+t")
    expect_mode(regular, channel, "+t")

    regular.send_line(f"TOPIC {channel} :forbidden")
    expect_numeric(regular, 482)

    operator.send_line(f"TOPIC {channel} :official topic")
    expect_topic(operator, operator.nick, channel, "official topic")
    expect_topic(regular, operator.nick, channel, "official topic")

    regular.send_line(f"TOPIC {channel}")
    expect_numeric(
        regular,
        332,
        lambda message: channel in message.params and "official topic" in message.params,
    )

    operator.send_line(f"MODE {channel} -t")
    expect_mode(operator, channel, "-t")
    expect_mode(regular, channel, "-t")

    regular.send_line(f"TOPIC {channel} :regular may edit")
    expect_topic(operator, regular.nick, channel, "regular may edit")
    expect_topic(regular, regular.nick, channel, "regular may edit")


def test_invite_only(ctx: TestContext) -> None:
    operator = ctx.register("operator", "o")
    regular = ctx.register("regular", "r")
    outsider = ctx.register("outsider", "x")
    channel = ctx.channel("invite")

    ctx.join(operator, channel)
    operator.drain()
    ctx.join(regular, channel)
    expect_join(operator, regular.nick, channel)
    operator.drain()
    regular.drain()

    operator.send_line(f"MODE {channel} +i")
    expect_mode(operator, channel, "+i")
    expect_mode(regular, channel, "+i")

    outsider.send_line(f"JOIN {channel}")
    expect_numeric(outsider, 473)

    regular.send_line(f"INVITE {outsider.nick} {channel}")
    expect_numeric(regular, 482)

    operator.send_line(f"INVITE {outsider.nick} {channel}")
    expect_numeric(operator, 341)
    expect_command(
        outsider,
        "INVITE",
        lambda message: prefix_nick(message.prefix) == operator.nick
        and len(message.params) >= 2
        and message.params[0] == outsider.nick
        and message.params[1] == channel,
        f"INVITE for {outsider.nick} to {channel}",
    )

    ctx.join(outsider, channel)
    expect_join(operator, outsider.nick, channel)
    expect_join(regular, outsider.nick, channel)

    outsider.send_line(f"PART {channel} :leave")
    expect_part(outsider, outsider.nick, channel)
    outsider.drain()
    outsider.send_line(f"JOIN {channel}")
    expect_numeric(outsider, 473)


def test_key_mode(ctx: TestContext) -> None:
    operator = ctx.register("operator", "o")
    second = ctx.register("second", "s")
    third = ctx.register("third", "t")
    channel = ctx.channel("key")
    key = "s3cret"

    ctx.join(operator, channel)
    operator.drain()
    operator.send_line(f"MODE {channel} +k {key}")
    expect_mode(operator, channel, "+k")
    assert_modes(operator, channel, present="k", args_present=[key])

    second.send_line(f"JOIN {channel}")
    expect_numeric(second, 475)
    second.send_line(f"JOIN {channel} wrong")
    expect_numeric(second, 475)
    ctx.join(second, channel, key)
    expect_join(operator, second.nick, channel)

    operator.send_line(f"MODE {channel} -k")
    expect_mode(operator, channel, "-k")
    assert_modes(operator, channel, absent="k")
    ctx.join(third, channel)


def test_limit_mode(ctx: TestContext) -> None:
    operator = ctx.register("operator", "o")
    second = ctx.register("second", "s")
    third = ctx.register("third", "t")
    channel = ctx.channel("limit")

    ctx.join(operator, channel)
    operator.drain()

    operator.send_line(f"MODE {channel} +l 1")
    expect_mode(operator, channel, "+l")
    assert_modes(operator, channel, present="l", args_present=["1"])

    second.send_line(f"JOIN {channel}")
    expect_numeric(second, 471)

    operator.send_line(f"MODE {channel} -l")
    expect_mode(operator, channel, "-l")
    ctx.join(second, channel)
    expect_join(operator, second.nick, channel)

    operator.send_line(f"MODE {channel} +l 2")
    expect_mode(operator, channel, "+l")
    assert_modes(operator, channel, present="l", args_present=["2"])
    third.send_line(f"JOIN {channel}")
    expect_numeric(third, 471)

    operator.send_line(f"MODE {channel} -l")
    expect_mode(operator, channel, "-l")
    operator.drain()
    operator.send_line(f"MODE {channel} +l 0")
    lines = operator.collect(ctx.args.settle)
    if any(parse_irc_line(line).command == "MODE" for line in lines):
        raise TestFailure("MODE +l 0 should be ignored, but a MODE broadcast was received")
    assert_modes(operator, channel, absent="l")


def test_operator_and_kick(ctx: TestContext) -> None:
    operator = ctx.register("operator", "o")
    regular = ctx.register("regular", "r")
    target = ctx.register("target", "t")
    channel = ctx.channel("kick")

    ctx.join(operator, channel)
    operator.drain()
    ctx.join(regular, channel)
    expect_join(operator, regular.nick, channel)
    ctx.join(target, channel)
    expect_join(operator, target.nick, channel)
    expect_join(regular, target.nick, channel)
    for client in (operator, regular, target):
        client.drain()

    regular.send_line(f"KICK {channel} {target.nick} :not allowed")
    expect_numeric(regular, 482)

    operator.send_line(f"MODE {channel} +o {regular.nick}")
    expect_mode(operator, channel, "+o")
    expect_mode(regular, channel, "+o")
    expect_mode(target, channel, "+o")

    regular.send_line(f"KICK {channel} {target.nick} :bye")
    expect_kick(operator, regular.nick, channel, target.nick)
    expect_kick(regular, regular.nick, channel, target.nick)
    expect_kick(target, regular.nick, channel, target.nick)

    target.send_line(f"PRIVMSG {channel} :still here?")
    expect_numeric(target, 442)

    operator.send_line(f"MODE {channel} -o {regular.nick}")
    expect_mode(operator, channel, "-o")
    expect_mode(regular, channel, "-o")

    regular.send_line(f"MODE {channel} +i")
    expect_numeric(regular, 482)


def test_empty_mode_parameter(ctx: TestContext) -> None:
    operator = ctx.register("operator", "o")
    channel = ctx.channel("empty")
    ctx.join(operator, channel)
    operator.drain()

    operator.send_line(f"MODE {channel} :")
    responses = operator.collect(ctx.args.settle)
    if responses:
        raise TestFailure(
            "MODE <channel> : was expected to be ignored, but received: "
            + " | ".join(responses)
        )

    operator.send_line("PING :still-alive")
    expect_command(
        operator,
        "PONG",
        lambda message: "still-alive" in message.params,
        "PONG after empty MODE parameter",
    )


def test_nick_quit_dedup(ctx: TestContext) -> None:
    first = ctx.register("first", "a")
    second = ctx.register("second", "b")
    channel_one = ctx.channel("sharedone")
    channel_two = ctx.channel("sharedtwo")

    ctx.join(first, channel_one)
    first.drain()
    ctx.join(second, channel_one)
    expect_join(first, second.nick, channel_one)

    ctx.join(first, channel_two)
    first.drain()
    ctx.join(second, channel_two)
    expect_join(first, second.nick, channel_two)

    first.drain()
    second.drain()
    old_nick = first.nick
    new_nick = ctx.unique_nick("n")
    first.send_line(f"NICK {new_nick}")

    first_lines = first.collect(ctx.args.settle)
    second_lines = second.collect(ctx.args.settle)

    def is_expected_nick(line: str) -> bool:
        message = parse_irc_line(line)
        return (
            message.command == "NICK"
            and prefix_nick(message.prefix) == old_nick
            and bool(message.params)
            and message.params[0] == new_nick
        )

    first_count = sum(1 for line in first_lines if is_expected_nick(line))
    second_count = sum(1 for line in second_lines if is_expected_nick(line))
    if first_count != 1:
        raise TestFailure(f"NICK changer received {first_count} NICK notifications; expected 1")
    if second_count != 1:
        raise TestFailure(f"shared client received {second_count} NICK notifications; expected 1")
    first.nick = new_nick

    second.drain()
    first.send_line("QUIT :dedup-check")
    quit_lines = second.collect(ctx.args.settle)

    def is_expected_quit(line: str) -> bool:
        message = parse_irc_line(line)
        return message.command == "QUIT" and prefix_nick(message.prefix) == new_nick

    quit_count = sum(1 for line in quit_lines if is_expected_quit(line))
    if quit_count != 1:
        raise TestFailure(f"shared client received {quit_count} QUIT notifications; expected 1")

    second.send_line("PING :after-quit")
    expect_command(
        second,
        "PONG",
        lambda message: "after-quit" in message.params,
        "PONG after peer QUIT",
    )

def test_multi_mode(ctx: TestContext) -> None:
    operator = ctx.register("operator", "o")
    second = ctx.register("second", "s")
    third = ctx.register("third", "t")
    fourth = ctx.register("fourth", "f")
    channel = ctx.channel("multi")
    key = "comboKey"

    ctx.join(operator, channel)
    operator.drain()
    ctx.join(second, channel)
    expect_join(operator, second.nick, channel)
    operator.drain()
    second.drain()

    operator.send_line(f"MODE {channel} +it")
    expect_mode(operator, channel, "+it")
    expect_mode(second, channel, "+it")
    assert_modes(operator, channel, present="it")

    operator.send_line(f"MODE {channel} -it")
    expect_mode(operator, channel, "-it")
    expect_mode(second, channel, "-it")
    assert_modes(operator, channel, absent="it")

    operator.send_line(f"MODE {channel} +kl {key} 3")
    expect_mode(operator, channel, "+kl")
    expect_mode(second, channel, "+kl")
    assert_modes(operator, channel, present="kl", args_present=[key, "3"])

    third.send_line(f"JOIN {channel} wrong")
    expect_numeric(third, 475)
    ctx.join(third, channel, key)
    expect_join(operator, third.nick, channel)
    expect_join(second, third.nick, channel)

    fourth.send_line(f"JOIN {channel} {key}")
    expect_numeric(fourth, 471)

    operator.send_line(f"MODE {channel} -kl")
    expect_mode(operator, channel, "-kl")
    expect_mode(second, channel, "-kl")
    expect_mode(third, channel, "-kl")
    assert_modes(operator, channel, absent="kl")
    ctx.join(fourth, channel)

    for client in (operator, second, third, fourth):
        client.drain()

    operator.send_line(f"MODE {channel} +oo {second.nick} {third.nick}")
    for client in (operator, second, third, fourth):
        expect_mode(client, channel, "+oo")

    second.send_line(f"MODE {channel} +i")
    for client in (operator, second, third, fourth):
        expect_mode(client, channel, "+i")

    third.send_line(f"MODE {channel} -i")
    for client in (operator, second, third, fourth):
        expect_mode(client, channel, "-i")

    operator.send_line(f"MODE {channel} -oo {second.nick} {third.nick}")
    for client in (operator, second, third, fourth):
        expect_mode(client, channel, "-oo")

    second.send_line(f"MODE {channel} +t")
    expect_numeric(second, 482)
    third.send_line(f"MODE {channel} +t")
    expect_numeric(third, 482)

    operator.send_line(f"MODE {channel} +l 5")
    for client in (operator, second, third, fourth):
        expect_mode(client, channel, "+l")
    assert_modes(operator, channel, present="l", args_present=["5"])

    operator.send_line(f"MODE {channel} +i-l")
    for client in (operator, second, third, fourth):
        expect_mode(client, channel, "+i-l")
    assert_modes(operator, channel, present="i", absent="l")



def test_slow_reader_flood(ctx: TestContext) -> None:
    """Verify that a client that temporarily stops reading does not block the server.

    This simulates the manual Ctrl+Z flood check without using job control:
    the victim joins a channel, then the test intentionally avoids reading from
    that socket while another client floods the channel.  A third client must
    still be able to register and PING the server.  Finally the victim starts
    reading again and must receive every flood message.
    """
    count = ctx.args.flood_count
    payload_size = ctx.args.flood_payload_size
    if count <= 0:
        raise TestFailure("--flood-count must be positive")
    if payload_size < 0:
        raise TestFailure("--flood-payload-size must be non-negative")

    victim = ctx.register("slow-victim", "v")
    channel = ctx.channel("flood")
    ctx.join(victim, channel)
    victim.drain()

    if ctx.args.flood_rcvbuf > 0:
        try:
            victim.sock.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, ctx.args.flood_rcvbuf)
            if ctx.trace is not None:
                actual_rcvbuf = victim.sock.getsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF)
                ctx.trace.event(f"FLOOD VICTIM RCVBUF | requested={ctx.args.flood_rcvbuf} actual={actual_rcvbuf}")
        except OSError as exc:
            if ctx.trace is not None:
                ctx.trace.event(f"FLOOD VICTIM RCVBUF | setsockopt failed: {exc}")

    sender = ctx.register("flood-sender", "f")
    ctx.join(sender, channel)
    sender.drain()

    # From this point until the final collection loop, do not call expect(),
    # collect(), or drain() on victim.  That is the automated equivalent of a
    # user pressing Ctrl+Z on an nc/IRC client: the TCP connection remains open,
    # but the peer process does not read from the socket.
    payload = "x" * payload_size
    lines: List[str] = []
    for i in range(count):
        lines.append(f"PRIVMSG {channel} :FLOODBOT {i:06d} {payload}\r\n")
    raw_payload = "".join(lines).encode("utf-8")

    # Sanity check: keep generated IRC messages below the 512-byte IRC line
    # limit including CRLF.  This avoids turning the flood test into a line
    # length error-path test.
    longest_line = max(len(line.encode("utf-8")) for line in lines)
    if longest_line > 512:
        raise TestFailure(
            f"flood line is {longest_line} bytes; reduce --flood-payload-size"
        )

    if ctx.trace is not None:
        ctx.trace.event(
            f"FLOOD START | {sender.label} -> {channel} | "
            f"messages={count} payload_size={payload_size} bytes={len(raw_payload)}"
        )
    sender.send_raw(raw_payload)
    if ctx.trace is not None:
        ctx.trace.event("FLOOD SENT | sender send_raw() completed")

    # If the server blocked while trying to write to victim, this independent
    # client will fail to register or fail to receive PONG.
    checker = ctx.register("flood-checker", "c")
    checker.send_line("PING :slow-reader-alive")
    expect_command(
        checker,
        "PONG",
        lambda message: "slow-reader-alive" in message.params,
        "PONG while slow reader has pending output",
    )

    # Now the victim starts reading again, like running `fg` after Ctrl+Z.
    received_ids: Set[int] = set()
    deadline = time.monotonic() + ctx.args.flood_drain_timeout
    all_lines: List[str] = []
    while time.monotonic() < deadline and len(received_ids) < count:
        for line in victim.collect(0.05):
            all_lines.append(line)
            message = parse_irc_line(line)
            if message.command != "PRIVMSG":
                continue
            if prefix_nick(message.prefix) != sender.nick:
                continue
            if len(message.params) < 2 or message.params[0] != channel:
                continue
            parts = message.params[1].split()
            if len(parts) >= 2 and parts[0] == "FLOODBOT" and parts[1].isdigit():
                received_ids.add(int(parts[1]))

    if len(received_ids) != count:
        # Build a short missing-id sample to keep the failure readable.
        missing_sample: List[int] = []
        for i in range(count):
            if i not in received_ids:
                missing_sample.append(i)
                if len(missing_sample) >= 10:
                    break
        raise TestFailure(
            f"slow reader flood lost messages: received {len(received_ids)}/{count}; "
            f"first missing ids={missing_sample}; victim closed={victim.closed}; "
            f"recent victim lines={all_lines[-5:]}"
        )

    if victim.closed:
        raise TestFailure("victim was disconnected during slow-reader flood test")

    if ctx.trace is not None:
        ctx.trace.event(f"FLOOD OK | victim received {len(received_ids)}/{count}")


TESTS: List[Tuple[str, Callable[[TestContext], None]]] = [
    ("registration", test_registration),
    ("join_messaging", test_join_and_messaging),
    ("slow_reader_flood", test_slow_reader_flood),
    ("topic_permissions", test_topic_permissions),
    ("invite_only", test_invite_only),
    ("key_mode", test_key_mode),
    ("limit_mode", test_limit_mode),
    ("operator_kick", test_operator_and_kick),
    ("empty_mode_parameter", test_empty_mode_parameter),
    ("nick_quit_dedup", test_nick_quit_dedup),
    ("multi_mode", test_multi_mode),
]

TEST_PLANS: Dict[str, List[Tuple[str, List[Tuple[str, str]]]]] = {
    "registration": [
        ("Block commands before registration", [
            ("SETUP", "open a TCP connection without PASS/NICK/USER"),
            ("SEND", "JOIN <channel>"),
            ("EXPECT", "451 You have not registered"),
        ]),
        ("Reject a wrong password", [
            ("SEND", "PASS definitely-wrong"),
            ("EXPECT", "464 Password incorrect"),
        ]),
        ("Complete normal registration", [
            ("SEND", "PASS <password>"),
            ("SEND", "NICK <unique-nick>"),
            ("SEND", "USER <username> 0 * :<realname>"),
            ("EXPECT", "001 welcome numeric"),
        ]),
        ("Reject re-registration and duplicate nicknames", [
            ("SEND", "PASS again after welcome"),
            ("EXPECT", "462 You may not reregister"),
            ("SETUP", "second client tries the first client's nickname"),
            ("EXPECT", "433 Nickname is already in use"),
        ]),
        ("Check PING/PONG after registration", [
            ("SEND", "PING :automation-probe"),
            ("EXPECT", "PONG containing automation-probe"),
        ]),
    ],
    "join_messaging": [
        ("Join two users to one channel", [
            ("SETUP", "register alice and bob"),
            ("SEND", "JOIN <channel> from both clients"),
            ("EXPECT", "JOIN broadcast and 353/366 NAMES replies"),
        ]),
        ("Forward channel messages only to other members", [
            ("SEND", "alice: PRIVMSG <channel> :hello channel"),
            ("EXPECT", "bob receives alice's PRIVMSG"),
            ("CHECK", "alice does not receive her own channel message"),
        ]),
        ("Deliver direct messages and PART", [
            ("SEND", "alice: PRIVMSG <bob> :private hello"),
            ("EXPECT", "bob receives the direct PRIVMSG"),
            ("SEND", "bob: PART <channel> :done"),
            ("EXPECT", "PART broadcast is visible to channel clients"),
        ]),
    ],
    "slow_reader_flood": [
        ("Create a slow reader", [
            ("SETUP", "victim joins one channel and then the tester stops reading from that socket"),
            ("SETUP", "sender joins the same channel"),
        ]),
        ("Flood while victim is not reading", [
            ("SEND", "sender sends --flood-count PRIVMSG lines in one burst"),
            ("CHECK", "a third client can still register and receive PONG"),
        ]),
        ("Resume reading", [
            ("CHECK", "victim receives every FLOODBOT message without disconnecting"),
        ]),
    ],
    "topic_permissions": [
        ("Enable topic restriction", [
            ("SETUP", "operator and regular user join one channel"),
            ("SEND", "operator: MODE <channel> +t"),
            ("EXPECT", "MODE +t broadcast"),
        ]),
        ("Enforce +t", [
            ("SEND", "regular: TOPIC <channel> :forbidden"),
            ("EXPECT", "482 You're not channel operator"),
            ("SEND", "operator changes TOPIC"),
            ("EXPECT", "TOPIC broadcast to both clients"),
        ]),
        ("Disable restriction", [
            ("SEND", "operator: MODE <channel> -t"),
            ("EXPECT", "MODE -t broadcast"),
            ("SEND", "regular changes TOPIC"),
            ("EXPECT", "TOPIC broadcast succeeds"),
        ]),
    ],
    "invite_only": [
        ("Create invite-only channel", [
            ("SETUP", "operator and regular user join; outsider stays outside"),
            ("SEND", "operator: MODE <channel> +i"),
            ("EXPECT", "MODE +i broadcast"),
        ]),
        ("Reject users without an invite", [
            ("SEND", "outsider: JOIN <channel>"),
            ("EXPECT", "473 Cannot join channel"),
            ("SEND", "regular: INVITE outsider <channel>"),
            ("EXPECT", "482 because regular is not operator in +i channel"),
        ]),
        ("Accept an operator invite once", [
            ("SEND", "operator: INVITE outsider <channel>"),
            ("EXPECT", "341 confirmation and INVITE delivery"),
            ("SEND", "outsider: JOIN <channel>"),
            ("EXPECT", "JOIN success"),
            ("CHECK", "after PART, the same outsider needs a new invite"),
        ]),
    ],
    "key_mode": [
        ("Set channel key", [
            ("SEND", "operator: MODE <channel> +k <key>"),
            ("EXPECT", "MODE +k broadcast"),
            ("CHECK", "MODE query includes k and the key value"),
        ]),
        ("Enforce key on JOIN", [
            ("SEND", "second: JOIN without key"),
            ("EXPECT", "475 Cannot join channel"),
            ("SEND", "second: JOIN with wrong key"),
            ("EXPECT", "475 Cannot join channel"),
            ("SEND", "second: JOIN with correct key"),
            ("EXPECT", "JOIN success"),
        ]),
        ("Remove key", [
            ("SEND", "operator: MODE <channel> -k"),
            ("EXPECT", "MODE -k broadcast"),
            ("CHECK", "third client can JOIN without key"),
        ]),
    ],
    "limit_mode": [
        ("Set and enforce user limit", [
            ("SEND", "operator: MODE <channel> +l 1"),
            ("EXPECT", "MODE +l broadcast and query value 1"),
            ("SEND", "second: JOIN <channel>"),
            ("EXPECT", "471 Cannot join channel"),
        ]),
        ("Remove limit and allow JOIN", [
            ("SEND", "operator: MODE <channel> -l"),
            ("EXPECT", "MODE -l broadcast"),
            ("CHECK", "second client can JOIN"),
        ]),
        ("Ignore invalid limit", [
            ("SEND", "operator: MODE <channel> +l 0"),
            ("EXPECT", "no MODE broadcast"),
            ("CHECK", "MODE query has no l flag"),
        ]),
    ],
    "operator_kick": [
        ("Check KICK permissions", [
            ("SETUP", "operator, regular user, and target join one channel"),
            ("SEND", "regular: KICK target"),
            ("EXPECT", "482 You're not channel operator"),
        ]),
        ("Grant operator and KICK", [
            ("SEND", "operator: MODE <channel> +o regular"),
            ("EXPECT", "MODE +o broadcast"),
            ("SEND", "regular: KICK target"),
            ("EXPECT", "KICK broadcast to channel clients"),
        ]),
        ("Remove operator", [
            ("SEND", "operator: MODE <channel> -o regular"),
            ("EXPECT", "MODE -o broadcast"),
            ("SEND", "regular: MODE <channel> +i"),
            ("EXPECT", "482 after demotion"),
        ]),
    ],
    "empty_mode_parameter": [
        ("Ignore explicit empty MODE string", [
            ("SETUP", "operator creates a channel"),
            ("SEND", "MODE <channel> :"),
            ("EXPECT", "no reply and no MODE broadcast"),
            ("SEND", "PING :still-alive"),
            ("EXPECT", "PONG proves the connection survived"),
        ]),
    ],
    "nick_quit_dedup": [
        ("Create duplicate-notification scenario", [
            ("SETUP", "two clients join two shared channels"),
            ("CHECK", "both clients share more than one channel"),
        ]),
        ("NICK must be sent once per client", [
            ("SEND", "first client changes nickname"),
            ("EXPECT", "changer receives exactly one NICK notification"),
            ("EXPECT", "shared client receives exactly one NICK notification"),
        ]),
        ("QUIT must be sent once per shared client", [
            ("SEND", "first client sends QUIT"),
            ("EXPECT", "shared client receives exactly one QUIT notification"),
            ("SEND", "remaining client PING"),
            ("EXPECT", "PONG after peer QUIT"),
        ]),
    ],
    "multi_mode": [
        ("Apply multiple flag modes", [
            ("SEND", "operator: MODE <channel> +it"),
            ("EXPECT", "one MODE +it broadcast"),
            ("CHECK", "MODE query contains i and t"),
            ("SEND", "operator: MODE <channel> -it"),
            ("EXPECT", "one MODE -it broadcast"),
        ]),
        ("Consume multiple mode values in order", [
            ("SEND", "operator: MODE <channel> +kl <key> 3"),
            ("EXPECT", "one MODE +kl broadcast"),
            ("CHECK", "key and limit are visible in MODE query"),
            ("CHECK", "wrong key gets 475; correct key can JOIN; full channel gets 471"),
        ]),
        ("Remove key and limit together", [
            ("SEND", "operator: MODE <channel> -kl"),
            ("EXPECT", "one MODE -kl broadcast"),
            ("CHECK", "MODE query no longer contains k or l"),
        ]),
        ("Grant and remove two operators in one command", [
            ("SEND", "operator: MODE <channel> +oo nick1 nick2"),
            ("EXPECT", "one MODE +oo broadcast"),
            ("CHECK", "both promoted users can change MODE"),
            ("SEND", "operator: MODE <channel> -oo nick1 nick2"),
            ("EXPECT", "one MODE -oo broadcast"),
            ("CHECK", "both demoted users get 482 for MODE"),
        ]),
        ("Handle mixed signs", [
            ("SEND", "operator: MODE <channel> +l 5"),
            ("SEND", "operator: MODE <channel> +i-l"),
            ("EXPECT", "one MODE +i-l broadcast"),
            ("CHECK", "i remains set and l is removed"),
        ]),
    ],
}

_BADGE_COLORS = {
    "SETUP": "blue",
    "SEND": "cyan",
    "EXPECT": "green",
    "CHECK": "yellow",
}


def format_test_plan_lines(name: str, style: Optional[Style] = None) -> List[str]:
    if style is None:
        style = Style(False)
    lines = [style.apply(f"[TEST] {name}", "bold", "magenta")]
    for step_index, (title, actions) in enumerate(TEST_PLANS.get(name, []), 1):
        lines.append("  " + style.apply(f"{step_index:02d}. {title}", "bold"))
        for kind, text in actions:
            color = _BADGE_COLORS.get(kind, "gray")
            badge = style.apply(kind.ljust(6), "bold", color)
            lines.append(f"      {badge} {text}")
    return lines


def print_test_plan(name: str, style: Optional[Style] = None) -> None:
    for line in format_test_plan_lines(name, style):
        print(line)


def write_test_plan(trace: TraceWriter, name: str) -> None:
    for line in format_test_plan_lines(name, Style(False)):
        trace.line(line)


class SpawnedServer:
    def __init__(self, executable: str, port: int, password: str) -> None:
        self.executable = executable
        self.port = port
        self.password = password
        self.process = None
        self.log_path = os.path.join(tempfile.gettempdir(), f"ft_irc_tester_{os.getpid()}.log")
        self.log_file = None

    def start(self, timeout: float) -> None:
        if not os.path.exists(self.executable):
            raise TestFailure(f"server executable not found: {self.executable}")
        if not os.access(self.executable, os.X_OK):
            raise TestFailure(f"server executable is not executable: {self.executable}")

        self.log_file = open(self.log_path, "wb")
        self.process = subprocess.Popen(
            [self.executable, str(self.port), self.password],
            stdout=self.log_file,
            stderr=subprocess.STDOUT,
        )

        deadline = time.monotonic() + max(2.0, timeout * 3)
        while time.monotonic() < deadline:
            if self.process.poll() is not None:
                self.log_file.flush()
                raise TestFailure(
                    f"server exited early with status {self.process.returncode}. "
                    f"Log: {self.log_path}"
                )
            try:
                probe = socket.create_connection(("127.0.0.1", self.port), timeout=0.15)
                probe.close()
                return
            except OSError:
                time.sleep(0.05)
        raise TestFailure(f"server did not listen on port {self.port}; log: {self.log_path}")

    def stop(self) -> None:
        if self.process is not None and self.process.poll() is None:
            try:
                self.process.send_signal(signal.SIGINT)
                self.process.wait(timeout=1.5)
            except (OSError, subprocess.TimeoutExpired):
                try:
                    self.process.terminate()
                    self.process.wait(timeout=1.0)
                except (OSError, subprocess.TimeoutExpired):
                    try:
                        self.process.kill()
                    except OSError:
                        pass
        if self.log_file is not None:
            self.log_file.close()

    def tail(self, line_count: int = 30) -> str:
        try:
            with open(self.log_path, "r", encoding="utf-8", errors="replace") as handle:
                lines = handle.readlines()
            return "".join(lines[-line_count:])
        except OSError:
            return ""


def check_server_reachable(host: str, port: int, timeout: float) -> None:
    try:
        probe = socket.create_connection((host, port), timeout=timeout)
        probe.close()
    except OSError as exc:
        raise TestFailure(
            f"cannot connect to {host}:{port}: {exc}. "
            "Start ./ircserv first or use --spawn ./ircserv."
        )


def parse_name_set(value: Optional[str]) -> Set[str]:
    if not value:
        return set()
    return {item.strip() for item in value.split(",") if item.strip()}


def color_enabled(mode: str) -> bool:
    if mode == "always":
        return True
    if mode == "never":
        return False
    return sys.stdout.isatty()


def make_trace_path(args: argparse.Namespace) -> str:
    if args.trace_file:
        return args.trace_file
    stamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"ft_irc_trace_{stamp}_{os.getpid()}.log"
    return os.path.join(args.trace_dir, filename)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Automated multi-client integration tests for ft_irc",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=(
            "Examples:\n"
            "  ./ircserv 6667 secret\n"
            "  python3 ft_irc_tester_trace.py --port 6667 --password secret\n\n"
            "  python3 ft_irc_tester_trace.py --spawn ./ircserv --port 6667 --password secret\n"
            "  python3 ft_irc_tester_trace.py --spawn ./ircserv --port 6667 --password secret --explain\n"
            "  python3 ft_irc_tester_trace.py --spawn ./ircserv --port 6667 --password secret --trace\n"
            "  python3 ft_irc_tester_trace.py --spawn ./ircserv --port 6667 --password secret --only multi_mode --explain --trace\n"
        ),
    )
    parser.add_argument("--host", default="127.0.0.1", help="server host (default: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=6667, help="server port (default: 6667)")
    parser.add_argument("--password", help="IRC server password")
    parser.add_argument("--spawn", metavar="PATH", help="start this ircserv executable automatically")
    parser.add_argument("--timeout", type=float, default=1.5, help="per-expect timeout in seconds")
    parser.add_argument("--settle", type=float, default=0.25, help="collection window for duplicate/no-response checks")
    parser.add_argument("--flood-count", type=int, default=1000, help="messages for slow_reader_flood (default: 1000)")
    parser.add_argument("--flood-payload-size", type=int, default=300, help="payload bytes per flood message (default: 300)")
    parser.add_argument("--flood-drain-timeout", type=float, default=10.0, help="seconds to wait for the slow reader to receive queued flood messages (default: 10.0)")
    parser.add_argument("--flood-rcvbuf", type=int, default=4096, help="victim SO_RCVBUF for slow_reader_flood; 0 disables tuning (default: 4096)")
    parser.add_argument("--only", help="comma-separated test names to run")
    parser.add_argument("--skip", help="comma-separated test names to skip")
    parser.add_argument("--list", action="store_true", help="list available tests and exit")
    parser.add_argument("--explain", action="store_true", help="print a colored test plan before each selected test")
    parser.add_argument("--trace", action="store_true", help="write a detailed trace file with plans and raw IRC traffic")
    parser.add_argument("--verbose", action="store_true", help="alias for --trace; kept for compatibility with the previous tester")
    parser.add_argument("--print-raw", action="store_true", help="also print raw IRC traffic live on stdout")
    parser.add_argument("--trace-dir", default="ft_irc_traces", help="directory for trace files (default: ft_irc_traces)")
    parser.add_argument("--trace-file", help="exact trace file path to write")
    parser.add_argument("--color", choices=["auto", "always", "never"], default="auto", help="colored output mode (default: auto)")
    parser.add_argument("--stop-on-fail", action="store_true", help="stop after the first failed test")
    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()
    style = Style(color_enabled(args.color))

    if args.list:
        for name, _ in TESTS:
            if args.explain:
                print_test_plan(name, style)
                print()
            else:
                print(name)
        return 0

    if not args.password:
        parser.error("--password is required unless --list is used")

    if not (1 <= args.port <= 65535):
        parser.error("--port must be between 1 and 65535")
    if args.timeout <= 0 or args.settle <= 0:
        parser.error("--timeout and --settle must be positive")
    if args.flood_count <= 0 or args.flood_payload_size < 0 or args.flood_drain_timeout <= 0 or args.flood_rcvbuf < 0:
        parser.error("--flood-count and --flood-drain-timeout must be positive; --flood-payload-size and --flood-rcvbuf must be non-negative")

    only = parse_name_set(args.only)
    skip = parse_name_set(args.skip)
    known = {name for name, _ in TESTS}
    unknown = (only | skip) - known
    if unknown:
        parser.error("unknown test name(s): " + ", ".join(sorted(unknown)))

    selected = [
        (name, function)
        for name, function in TESTS
        if (not only or name in only) and name not in skip
    ]
    if not selected:
        parser.error("no tests selected")

    trace_writer: Optional[TraceWriter] = None
    if args.trace or args.verbose:
        trace_writer = TraceWriter(make_trace_path(args))

    spawned: Optional[SpawnedServer] = None
    failures = 0
    passed = 0
    started = time.monotonic()

    try:
        if trace_writer is not None:
            trace_writer.header(args, [name for name, _ in selected])

        if args.spawn:
            spawned = SpawnedServer(args.spawn, args.port, args.password)
            spawned.start(args.timeout)
            # A spawned server is local even if --host was accidentally changed.
            args.host = "127.0.0.1"
            if trace_writer is not None:
                trace_writer.line(f"server_log: {spawned.log_path}")
                trace_writer.line("")
        else:
            check_server_reachable(args.host, args.port, args.timeout)

        print(style.apply(f"Target: {args.host}:{args.port}", "bold", "cyan"))
        print(style.apply(f"Running {len(selected)} test(s)", "bold"))
        if trace_writer is not None:
            print(style.apply(f"Trace: {trace_writer.path}", "bold", "magenta"))
        print()

        for index, (name, function) in enumerate(selected, 1):
            context = TestContext(args, name, index, trace_writer)
            case_started = time.monotonic()
            if args.explain:
                print_test_plan(name, style)
            if trace_writer is not None:
                trace_writer.test_start(index, len(selected), name)
                write_test_plan(trace_writer, name)
                trace_writer.line("")
            try:
                function(context)
            except (TestFailure, OSError, ValueError) as exc:
                failures += 1
                elapsed = time.monotonic() - case_started
                print(style.apply(f"[FAIL] {name} ({elapsed:.2f}s)", "bold", "red"))
                print(f"  {exc}")
                if trace_writer is not None:
                    trace_writer.test_result("FAIL", name, elapsed, str(exc))
                transcript = context.transcript()
                if transcript and not args.print_raw and trace_writer is None:
                    print("  Recent per-client traffic:")
                    print(transcript)
                if args.stop_on_fail:
                    context.close()
                    break
            except Exception as exc:  # Keep the runner useful even if a test itself has a bug.
                failures += 1
                elapsed = time.monotonic() - case_started
                print(style.apply(f"[ERROR] {name} ({elapsed:.2f}s)", "bold", "red"))
                print(f"  {type(exc).__name__}: {exc}")
                if trace_writer is not None:
                    trace_writer.test_result("ERROR", name, elapsed, f"{type(exc).__name__}: {exc}")
                transcript = context.transcript()
                if transcript and not args.print_raw and trace_writer is None:
                    print(transcript)
                if args.stop_on_fail:
                    context.close()
                    break
            else:
                passed += 1
                elapsed = time.monotonic() - case_started
                print(style.apply(f"[PASS] {name} ({elapsed:.2f}s)", "bold", "green"))
                if trace_writer is not None:
                    trace_writer.test_result("PASS", name, elapsed)
            finally:
                context.close()

    except TestFailure as exc:
        print(style.apply(f"[FATAL] {exc}", "bold", "red"), file=sys.stderr)
        failures += 1
        if trace_writer is not None:
            trace_writer.event(f"FATAL | {exc}")
    finally:
        if spawned is not None:
            spawned.stop()
        total_elapsed = time.monotonic() - started
        if trace_writer is not None:
            trace_writer.line("")
            trace_writer.line(f"SUMMARY: {passed} passed, {failures} failed ({total_elapsed:.2f}s)")
            trace_writer.close()

    total_elapsed = time.monotonic() - started
    result_line = f"Result: {passed} passed, {failures} failed ({total_elapsed:.2f}s)"
    if failures == 0:
        print("\n" + style.apply(result_line, "bold", "green"))
    else:
        print("\n" + style.apply(result_line, "bold", "red"))
    if trace_writer is not None:
        print(style.apply(f"Trace saved to: {trace_writer.path}", "bold", "magenta"))
    if spawned is not None and failures:
        tail = spawned.tail()
        if tail:
            print(f"\nServer log tail ({spawned.log_path}):\n{tail}")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())

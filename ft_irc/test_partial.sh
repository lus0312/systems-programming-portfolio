#!/bin/sh

{
	printf 'pa'
	sleep 1
	printf 'ss secret\r\n'
	sleep 1
	printf 'ni'
	sleep 1
	printf 'ck happy\r\n'
	sleep 1
	printf 'us'
	sleep 1
	printf 'er h 0 * :real\r\n'
	sleep 1
	printf 'jo'
	sleep 1
	printf 'in #42\r\n'
	sleep 1
	printf 'quit\r\n'
} | nc -C 127.0.0.1 6667

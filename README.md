# 시스템 · 네트워크 프로그래밍 포트폴리오

42경산 교육 과정에서 수행한 프로젝트 중, 프로세스 · 네트워크 통신 · 동시성 제어 · 서비스 구성을 다룬 다섯 가지를 정리했습니다. 각 문서에는 구현 내용과 함께 **직접 마주친 문제와 해결 과정, 동작을 확인한 방법**을 함께 담았습니다.

## 다루는 주제

| 분야 | 내용 |
| --- | --- |
| 프로세스 · 시스템 콜 | `fork` / `execve` / `pipe` / `dup2` / `waitpid`, 시그널 처리, 파일 디스크립터와 자원 정리, 종료 상태 |
| 네트워크 | TCP 소켓, 논블로킹 I/O, `poll()` 이벤트 루프, 메시지 경계 처리, 부분 송수신 |
| 동시성 | POSIX 스레드, mutex 기반 공유 자원 보호, 자원 획득 순서, 종료 조건과 자원 해제 |
| 운영 환경 | Docker 이미지 · 컨테이너 구성, 서비스 분리와 내부 통신, 영속 저장소, TLS 설정 |
| 언어 · 도구 | C, C++98, Linux, GNU Make, Docker Compose, Valgrind, Git |

## 프로젝트

| 프로젝트 | 개요 | 해결한 문제 |
| --- | --- | --- |
| [minishell](minishell/README.md) | C · 프로세스, 파이프, 시그널을 다루는 bash 축소판 (2인 팀) | 같은 Ctrl-C가 프롬프트 · 실행 중 · heredoc에서 다르게 동작해야 하는 문제를 전역 상태 값 하나로 구분하고, heredoc 입력을 별도 프로세스로 분리 |
| [pipex](pipex/README.md) | C · 셸의 파이프(`< in cmd1 \| cmd2 > out`)를 fork · pipe · dup2로 구현 (개인) | 파이프의 쓰기 끝이 하나라도 열려 있으면 EOF가 오지 않는 원리를 이해하고, 셸의 동작을 기준으로 구현 범위와 오류 처리를 확정 |
| [ft_irc](ft_irc/README.md) | C++98 논블로킹 TCP 기반 IRC 서버 (3인 팀) | 수신이 지연되는 클라이언트 때문에 송신이 막히는 문제를 연결별 송신 버퍼와 `POLLOUT` 재시도로 해결 |
| [Philosophers](philosophers/README.md) | C · pthread 기반 식사하는 철학자 시뮬레이션 | 스레드 생성이 중간에 실패할 때 사용 중인 자원이 먼저 해제되는 경로를 발견하고 종료 순서를 재구성 |
| [Inception](inception/README.md) | Docker로 Nginx · WordPress · MariaDB 등 5개 서비스 구성 | 볼륨 이름과 실제 호스트 저장 경로의 역할 차이를 확인하고 실행 전 디렉터리 준비 과정을 정리 |

### minishell — 프로세스와 시그널을 다루는 셸

C로 bash를 축소 구현했고, 그중 실행부(프로세스 생성과 회수, 파이프, 리다이렉션 fd 처리, 시그널, heredoc, 종료 상태)를 맡았습니다. 입력을 프로세스로 실행하고 파이프로 연결하는 구조와, Ctrl-C · Ctrl-D 같은 중단 상황에서 셸이 죽지 않게 하는 상태 관리를 다뤘습니다. bash와 출력 · 종료 상태를 나란히 비교하고 Valgrind로 누수를 확인했습니다.

### pipex — 셸의 파이프 구현

`< file1 cmd1 | cmd2 > file2`를 C로 재현했습니다. `pipe`가 만드는 커널 버퍼와 `dup2`의 fd 바꿔치기를 이용해 두 프로세스를 연결하고, `fork`로 복사된 fd를 누가 닫아야 하는지를 다뤘습니다. 구현 범위가 모호한 부분은 실제 셸의 동작을 기준으로 정하고, 같은 입력의 결과와 종료 코드를 나란히 비교해 확인했습니다.

### ft_irc — 다중 클라이언트 IRC 서버

C++98로 IRC 프로토콜의 인증 · 채널 · 메시지 전달을 구현했습니다. 하나의 `poll()` 루프에서 여러 연결의 접속 · 수신 · 송신 · 종료를 처리하며, TCP가 메시지 경계를 보장하지 않는다는 점을 전제로 수신 버퍼 누적과 송신 버퍼 재시도 구조를 설계했습니다. 실제 IRC 클라이언트(`irssi`)와 공개 서버(Libera.Chat)의 동작을 비교 기준으로 삼아 검증했습니다.

### Philosophers — 스레드와 동기화

철학자별 스레드와 포크별 mutex로 공유 자원 접근을 제어하고, 감시 루프에서 사망 조건과 최소 식사 횟수를 확인하도록 구성했습니다. 공유 상태는 쓰는 쪽뿐 아니라 읽는 쪽에서도 보호해야 한다는 점, mutex 사용과 자원 획득 순서 설계가 별개의 문제라는 점을 구현을 통해 확인했습니다.

### Inception — 컨테이너 기반 서비스 환경

Debian 기반 이미지를 직접 작성해 Nginx, WordPress/PHP-FPM, MariaDB, Redis, FTP를 각각의 컨테이너로 분리하고 Compose 네트워크로 연결했습니다. 비밀번호는 Compose secrets로 전달하고, 데이터는 볼륨으로 분리해 컨테이너의 수명과 데이터의 수명을 구분했습니다.

## 그 외 42 Common Core 과제

위 프로젝트 외에 수행한 과제도 [`42-common-core/`](42-common-core/README.md)에 모두 담았습니다. 과제별 개요와 실행 방법은 각 폴더의 README에서 볼 수 있습니다.

| 분류 | 과제 |
| --- | --- |
| C 기초 · 라이브러리 | [libft](42-common-core/commoncore_00/libft), [ft_printf](42-common-core/commoncore_01/ft_printf), [get_next_line](42-common-core/commoncore_01/get_next_line) |
| 알고리즘 · 그래픽 | [push_swap](42-common-core/commoncore_02/push_swap), [fract-ol](42-common-core/commoncore_02/fractol) |
| 네트워크 · 시스템 관리 | [NetPractice](42-common-core/commoncore_04/NetPractice), [Born2beRoot](42-common-core/commoncore_01/born2beroot) |
| C++ | [CPP Module 00~04](42-common-core/commoncore_04/CPP%20Module), [CPP Module 05~09](42-common-core/commoncore_05) |

---

각 프로젝트 문서에 과제 개요, 설계, 주요 구현, 문제 해결 과정, 실행 방법을 정리했습니다.

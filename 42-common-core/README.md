# 42 Common Core 과제 모음

42경산 Common Core 커리큘럼(Rank 00~05)에서 수행한 과제 코드입니다.
시스템 · 네트워크 · 동시성 · 인프라를 다룬 네 프로젝트(minishell, ft_irc, Philosophers, Inception)는 저장소 루트에서 더 자세히 정리했고, 이 폴더에는 나머지 과제와 함께 전체 목록을 담았습니다.
폴더마다 짧은 README가 있어 필요한 과제만 골라 볼 수 있습니다.

## 먼저 보면 좋은 과제

| 과제 | 볼 만한 이유 |
|---|---|
| [minishell](../minishell) | 프로세스 · 파일 디스크립터 · 시그널을 직접 다루는 bash 축소판 (파서, 파이프, 리다이렉션, 빌트인) |
| [ft_irc](../ft_irc) | C++98 IRC 서버. 논블로킹 소켓과 `poll()` 기반 이벤트 루프 |
| [Inception](../inception) | Docker Compose로 Nginx · WordPress · MariaDB 인프라 구성 (Redis, FTP 포함) |
| [Philosophers](../philosophers) | 스레드, 뮤텍스, 데드락 회피, 감시 루프 |

## 전체 과제 목록

| Rank | 과제 | 한 줄 요약 | 언어 / 기술 |
|---|---|---|---|
| 00 | [libft](commoncore_00/libft) | 표준 C 라이브러리 함수를 직접 구현한 개인 라이브러리 | C |
| 01 | [ft_printf](commoncore_01/ft_printf) | `printf`의 변환 지정자(`cspdiuxX%`) 구현, 가변 인자 | C |
| 01 | [get_next_line](commoncore_01/get_next_line) | fd에서 한 줄씩 읽는 함수, 정적 변수로 상태 유지 | C |
| 01 | [Born2beRoot](commoncore_01/born2beroot) | 가상 머신 리눅스 서버 구축 · 보안 설정 (제출물은 서명 해시) | Linux, SSH, UFW, sudo |
| 02 | [push_swap](commoncore_02/push_swap) | 제한된 명령어로 스택을 정렬하는 알고리즘 과제 | C, 연결 리스트, radix sort |
| 02 | [pipex](../pipex) | `< file1 cmd1 \| cmd2 > file2`를 재현하는 파이프 프로그램 | C, fork/pipe/dup2/execve |
| 02 | [fract-ol](commoncore_02/fractol) | Mandelbrot / Julia 프랙탈 그래픽 | C, MiniLibX |
| 03 | [Philosophers](../philosophers) | 식사하는 철학자 문제 (동기화) | C, pthread, mutex |
| 03 | [minishell](../minishell) | 나만의 작은 셸 | C, readline, 프로세스/시그널 |
| 04 | [NetPractice](commoncore_04/NetPractice) | 서브넷 · 라우팅 설정 퀴즈 10문제 | 네트워크 기초 |
| 04 | [CPP Module 00~04](commoncore_04/CPP%20Module) | C++98 기초: 클래스, 상속, 다형성 | C++98 |
| 05 | [CPP Module 05~09](commoncore_05) | 예외, 캐스팅, 템플릿, STL | C++98 |
| 05 | [ft_irc](../ft_irc) | IRC 서버 (3인 팀) | C++98, socket, poll |
| 05 | [Inception](../inception) | Docker 기반 웹 인프라 | Docker, Nginx, MariaDB, WordPress |

## 사용 안내

- 모든 C/C++ 과제는 각 폴더에서 `make`로 빌드합니다. `get_next_line`은 Makefile이 없어 `cc -D BUFFER_SIZE=42 ...`로 직접 컴파일합니다.
- 빌드 산출물(`*.o`, `*.a`, 실행 파일)은 저장소에 포함하지 않습니다.
- 일부 폴더에는 과제에서 제공되는 MiniLibX 사본(`minilibx-linux`)이 들어 있습니다.

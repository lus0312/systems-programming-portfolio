# get_next_line

파일 디스크립터에서 **한 줄씩** 읽어 반환하는 `get_next_line(int fd)` 구현입니다.

## 동작 방식

1. `read()`로 `BUFFER_SIZE`만큼 읽어 정적 변수(`backup`)에 이어 붙입니다.
2. 개행이 포함되면 읽기를 멈춥니다. 이미 완성된 줄이 남아 있으면 `read()`를 다시 호출하지 않습니다.
3. `backup`에서 한 줄을 잘라 반환하고, 나머지는 다음 호출을 위해 남깁니다.

## 구성

| 파일 | 내용 |
|---|---|
| `get_next_line.c/.h`, `get_next_line_utils.c` | 필수 파트 |
| `get_next_line_bonus.c/.h`, `get_next_line_utils_bonus.c` | 보너스: 정적 변수 하나로 **여러 fd 동시 처리** |
| `main.c`, `main_bonus.c`, `tests/` | 직접 만든 테스트 |

## 핵심 개념

정적 변수, 버퍼 크기에 무관한 동작(1 ~ 매우 큰 값), 파일·표준입력 읽기, 메모리 누수 방지

## 빌드 (Makefile 없음)

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

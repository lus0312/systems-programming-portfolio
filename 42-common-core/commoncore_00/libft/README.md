# libft

C 표준 라이브러리의 일부를 직접 구현해 만든 개인 라이브러리입니다. 이후 대부분의 C 과제에서 재사용합니다.

## 구성

- **libc 재구현**: `ft_strlen`, `ft_strchr`, `ft_strncmp`, `ft_memcpy`, `ft_memmove`, `ft_atoi`, `ft_calloc`, `ft_isalpha` 등
- **추가 함수**: `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_itoa`, `ft_strmapi`, `ft_striteri`, `ft_put*_fd`
- **보너스**: 연결 리스트 함수 (`ft_lstnew`, `ft_lstadd_back`, `ft_lstmap`, `ft_lstclear` 등)

## 핵심 개념

메모리 할당과 해제, 포인터 연산, `memmove`의 겹치는 영역 처리, 문자열 경계 조건, Makefile 작성

## 빌드

```bash
make          # libft.a 생성
make bonus    # 연결 리스트 함수 포함
```

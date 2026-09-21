# ft_printf

`printf`의 핵심 기능을 직접 구현한 라이브러리(`libftprintf.a`)입니다.

## 지원 변환

`%c` `%s` `%p` `%d` `%i` `%u` `%x` `%X` `%%`

## 핵심 개념

가변 인자(`stdarg.h`), 정수를 진법 변환해 출력하는 재귀/반복 처리, 포인터 주소 출력, `write` 반환값 누적

## 구성

- `ft_printf.c` : 포맷 문자열 파싱과 변환 지정자 분기
- `ft_printf_utils.c` : 숫자/문자열/16진수 출력 보조 함수
- `libft/` : 이전 과제의 libft 사본

## 빌드

```bash
make    # libftprintf.a 생성
```

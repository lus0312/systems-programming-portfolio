# CPP Module 06 - C++ 형 변환

C++98 모듈 과제입니다. 이번 모듈의 핵심: 네 가지 C++ 캐스트(`static_cast`, `dynamic_cast`, `reinterpret_cast`, `const_cast`).

| 연습 | 내용 |
|---|---|
| ex00 | `ScalarConverter`: 문자열 리터럴을 char/int/float/double로 변환 |
| ex01 | `Serializer`: 포인터 ↔ `uintptr_t` 직렬화 |
| ex02 | `Base`/`A`/`B`/`C`: RTTI 없이 `dynamic_cast`로 실제 타입 판별 |

## 빌드

각 `exNN` 폴더에서 `make`를 실행합니다 (`-Wall -Wextra -Werror -std=c++98`).

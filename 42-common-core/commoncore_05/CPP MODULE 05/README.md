# CPP Module 05 - 예외 처리

C++98 모듈 과제입니다. 이번 모듈의 핵심: C++ 예외(`try/catch`, 사용자 정의 예외 클래스)와 중첩 클래스.

| 연습 | 내용 |
|---|---|
| ex00 | `Bureaucrat`: 등급(1~150)을 검증하고 범위를 벗어나면 예외 발생 |
| ex01 | `Form`: 서명 가능 등급 검사와 `beSigned` |
| ex02 | 추상 클래스 `AForm`과 구체 양식 3종(`ShrubberyCreation`, `RobotomyRequest`, `PresidentialPardon`) |
| ex03 | `Intern`: 이름 문자열로 알맞은 양식을 생성하는 팩토리 |

## 빌드

각 `exNN` 폴더에서 `make`를 실행합니다 (`-Wall -Wextra -Werror -std=c++98`).

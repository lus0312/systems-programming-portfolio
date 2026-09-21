# push_swap

스택 두 개(`a`, `b`)와 제한된 명령어만으로 정수 목록을 **최소 명령 수**로 정렬하는 알고리즘 과제입니다.

## 명령어

`sa sb ss` (swap) · `pa pb` (push) · `ra rb rr` (rotate) · `rra rrb rrr` (reverse rotate)

## 접근 방식

- 입력을 **연결 리스트** 스택으로 관리 (`linked_list.c`)
- 값을 순위(0~n-1)로 바꾸는 **인덱스 압축** (`stack_ranking.c`)
- 큰 입력: 비트 단위 **radix sort** (`radix_sort.c`)
- 작은 입력(2~5개 등): 별도의 간단한 정렬 (`simple_ver.c`)
- 인자 검증(중복, 범위 초과, 숫자 아님)과 에러 처리 (`check_args.c`, `error_and_free.c`)

## 핵심 개념

알고리즘 복잡도와 명령 수 최적화, 연결 리스트, 입력 검증, 메모리 해제

## 실행

```bash
make
./push_swap 3 2 1 5 4
```

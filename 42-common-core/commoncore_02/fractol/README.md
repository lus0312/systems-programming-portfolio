# fract-ol

MiniLibX로 프랙탈을 그리는 그래픽 과제입니다.

## 구현

- **Mandelbrot**, **Julia** 집합 렌더링
- 키보드/마우스 이벤트 처리 (`events.c`): 창 닫기, 조작 입력
- 이미지 버퍼에 픽셀을 직접 기록해 그리는 방식 (`fractol.c`, `init.c`)

## 핵심 개념

복소수 반복 계산(`z = z² + c`)과 발산 판정, 좌표 변환, MiniLibX 이벤트 훅과 이미지 버퍼

## 실행

```bash
make
./fractol mandelbrot
```

`minilibx-linux/`는 과제에서 제공되는 그래픽 라이브러리 사본입니다.

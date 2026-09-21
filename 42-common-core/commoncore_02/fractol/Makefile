CC = gcc
# 컴파일러 옵션 (컴파일에만 사용)
CFLAGS = -Wall -Wextra -Werror -O3 -march=native -I$(MLX_DIR) -I/usr/include

# 링크 옵션 (링크에만 사용)
LDFLAGS = -L$(MLX_DIR) -lmlx_Linux -lXext -lX11 -lm

MLX_DIR = minilibx-linux

MLX_LIB = $(MLX_DIR)/libmlx_Linux.a

SRC = fractol.c events.c init.c render.c string_utils.c

OBJ = $(SRC:.c=.o)

NAME = fractol

all: $(NAME)

$(NAME): $(OBJ)
	make -C ./minilibx-linux
	$(CC) $(OBJ) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re


NAME = push_swap

CFLAGS = -Wall -Wextra -Werror

SRCS = push_swap.c \
	   check_args.c  \
	   commands_p.c \
	   commands_r.c \
	   commands_s.c \
	   error_and_free.c \
	   ft_atoi.c \
	   ft_putendl_fd.c \
	   ft_split.c \
	   ft_strchr.c \
	   ft_strdup.c \
	   ft_strlen.c \
	   ft_substr.c \
	   handle_empty.c \
	   is_sorted.c \
	   linked_list.c \
	   radix_sort.c \
	   simple_ver.c \
	   stack_ranking.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re

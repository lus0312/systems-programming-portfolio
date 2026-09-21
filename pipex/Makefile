NAME = pipex

CC = cc

CFLAGS = -Wall -Werror -Wextra

SRCS = pipex.c ft_split.c pipex_utils_1.c pipex_utils_2.c pipex_utils_3.c pipex_utils_4.c pipex_utils_5.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS) 
		$(CC) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
		rm -f $(OBJS)

fclean: clean 
		rm -f $(NAME)

re: 
	$(MAKE) fclean
	$(MAKE) all

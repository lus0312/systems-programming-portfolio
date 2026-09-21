NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
CFLAGS += -I$(INC_DIR)

SRC_DIR := src
INC_DIR := includes

SRCS := main.c		utils.c \
	ft_lst.c	ft_cstring.c	  ft_stdlib.c \
	ft_advstr.c	ft_excute_main.c  ft_excute_utils.c\
	handle_signal.c		handle_terminal.c \
	handle_fd.c		handle_path.c\
	check_cmd.c		check_utils.c \
	parse_input.c	parse_env.c	  parse_quote.c \
	parse_data_main.c	parse_data_utils.c parse_data_glue.c \
	parse_file_main.c	parse_file_utils.c \
	parse_file_divide.c parse_file_heredoc.c \
	run_minishell.c	run_pipe_main.c   run_pipe_utils.c \
	builtin_cd_main.c	builtin_cd_utils.c \
	builtin_export_main.c	builtin_export_utils.c \
	builtin_unset.c	builtin_env.c	builtin_echo.c \
	builtin_pwd.c	builtin_exit.c	builtin_handle.c \

OBJS := $(SRCS:%.c=$(SRC_DIR)/%.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re

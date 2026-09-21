#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <termios.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/ioctl.h>
# include <sys/resource.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define PATH_MAX 4096

extern volatile sig_atomic_t	g_sig_status;

enum e_role_value
{
	ENV = 1,
	UNSET,
	CMD,
	PCMD,
	GLUE,
	REDIR_NONE = 50,
	REDIR_EXIST,
	REDIR_READ,
	REDIR_HERE,
	REDIR_TRUNC,
	REDIR_APPEND,
	BUILTIN_ECHO = 100,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT,
};

typedef struct s_command_handler
{
	int		i;
	int		s;
	int		role;
	char	*temp;
}		t_cmd;

typedef struct s_list
{
	int				role;
	int				redir;
	int				fd;
	char			*data;
	struct s_list	*next;
}		t_list;

typedef struct s_pipe_handler
{
	int		p_len;
	int		**fd;
	pid_t	*pid;
}		t_ph;

typedef struct s_data
{
	int		status;
	int		quoted;
	char	**cmd;
	t_list	**input;
	t_list	*now;
	t_list	**env;
}		t_data;

// check_cmd.c
int		ft_check_right_cmd(char *line, int *status);

// check_utils.c
void	ft_pass_quotation(char *line, int *i);
char	**ft_dup_str(char *str);

// utils.c
void	ft_data_clear(char **cmd, t_list **input, t_list **env);
void	ft_free_str(char **arr);
void	ft_free_int(int **arr, int len);
void	ft_prt_err(t_data *data, char *str, int status);
int		ft_err_rtn(char *str, int rtn);

// handle_signal.c
void	ft_signal_int_handler(int signum);
void	ft_signal_quit_handler(int signum);

// handle_terminal.c
int		ft_turn_off_echo(void);
int		ft_turn_on_echo(void);

// handle_path.c
int		ft_ispath(char *str);
char	*ft_handle_path(char *str, int cd);

// handel_fd.c
void	ft_close_fd(t_data *data);
void	ft_handle_fd_origin(t_data *data);

// run_minishell.c
int		ft_run_minishell(char **line, t_list **env, int *status);

// run_pipe_main.c
t_list	*ft_excute_pipe(t_data *data, t_list **cmd);

// run_pipe_utils.c
int		ft_set_pipe_arg(t_list *cmd, t_ph *ph);
void	ft_free_pipe_arg(t_ph *ph, int i);
void	ft_handle_child_pipe(t_ph *ph, int end);
void	ft_set_exit_status(int *status, int raw);
void	ft_wait_pipe(t_data *data, t_ph *ph);

// ft_cstring.c
int		ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, int n);
char	*ft_strdup(const char *src);
char	*ft_strndup(const char *src, int size);

// ft_advstr.c
char	**ft_split(const char *s, char c);
char	*ft_straddjoin(const char *s1, const char *s2, char add);
char	*ft_strreplace(char *str, int start, int end, char *rep);

// ft_stdlib.c
char	*ft_itoa(int num);
int		ft_atoi(const char *nptr, int *err);
int		ft_realloc(char **temp, int *size);

// ft_lst.c
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstadd_back(t_list **lst, char *data, int role);
void	ft_lstclear(t_list **lst);
char	*ft_lsttostr(t_list **lst);

// ft_excute_main.c
void	ft_origin_excute(t_data *data);

// ft_excute_utils.c
char	**ft_lsttodata(t_list *env, int is_env);
char	*ft_get_env_path(t_list **env);
char	*ft_get_path(t_data *data, char *cmd, char *env);

// parse_input.c
t_list	*ft_parse_input(char **line);

// parse_data_main.c
char	**ft_parse_data(t_data *data, char *cmd);

// parse_data_glue.c
char	*ft_expand_token(t_data *data, char *token, int *status);
int		ft_merge_glue(t_list *div);

// parse_data_utils.c
int		ft_need_divide(char c);
t_list	*ft_divide_quote(t_list **div, char *cmd, t_cmd *h);
t_list	*ft_divide_redirect(t_list **div, char *cmd, t_cmd *h);
t_list	*ft_divide_env(t_list **div, char *cmd, t_cmd *h);
t_list	*ft_divide_special_block(char *cmd, t_list **div, t_cmd *h);

// parse_env.c
int		ft_isenv(char c);
char	*ft_get_value(char *key, t_list *env, int status);
char	*ft_parse_env(char *token, t_list *env, int *status);

// parse_quote.c
char	*ft_parse_quote(char *token, t_list *env, int status);

// parse_file_main.c
int		ft_parse_redirect(t_data *cmd, t_list **div, \
		char *token, int *status);

// parse_file_divide.c
int		ft_has_quote(char *str);
t_list	*ft_divide_redirect_token(char *token, t_list **div);

// parse_file_utils.c
int		ft_parse_file_in(t_data *data, t_list **div, \
		char *redirect, int role);
int		ft_parse_file_out(t_list **cmd, char *redirect, int role);
int		ft_is_delimiter(char *line, char *eof);
void	ft_heredoc_write(t_data *data, char *line);

// parse_file_heredoc.c
int		ft_redirect_heredoc(t_data *data, t_list **div, \
		char *eof);

/* builtins */

// builtin_handle.c
int		ft_is_builtin(char *cmd);
void	ft_builtin_excute(int builtin, t_data *data);

// builtin_pwd.c
void	ft_builtin_pwd(t_data *data);

// builtin_exit.c
void	ft_excute_builtin_exit(t_data *data, int num, int isexit);
void	ft_builtin_exit(t_data *data);

// builtin_unset.c
void	ft_builtin_unset(t_data *data);

// builtin_echo.c
void	ft_builtin_echo(t_data *data);

// builtin_env.c
void	ft_builtin_env(t_data *data);

// builtin_export_main.c
void	ft_builtin_export(t_data *data);

// builtin_export_utils.c
void	ft_export_result(t_data *data);
int		ft_canexport(char *s);
void	ft_export_invalid(t_data *data, char *str);

// builtin_cd_main.c
void	ft_builtin_cd(t_data *data);

// builtin_cd_utils.c
char	*ft_handle_parameter(char *str, t_list *env, int *print);
void	ft_cd_result(t_data *data, char *pwd);

#endif

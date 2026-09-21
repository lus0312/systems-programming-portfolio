#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <sys/types.h> 
# include <sys/wait.h>

typedef struct s_pipex
{
	int		infile;
	int		outfile;
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	char	**cmd1;
	char	**cmd2;
	char	**paths;

}	t_pipex;

char	**ft_split(char const *s, char c);
int		countrows(char const *s, char c);
void	memory_free(char **res, unsigned int i);
char	**get_word(char const *s, char c, char **res);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t num);
size_t	ft_strlen(const char *str);
int		ft_printf(const char *str);
void	free_array(char **array);
void	clean_up(t_pipex *pipex);
void	handle_error(char *str, t_pipex *pipex);
void	open_files(t_pipex *pipex, char **argv);
void	init_t_pipex(t_pipex *pipex);
void	execute_cmd(t_pipex *pipex, int num, char **envp);
char	**parse_cmd(char *cmd);
char	**split_cmd(char const *s);
void	cmd_error(t_pipex *pipex, char **cmd, char *msg, int code);
void	exec_fail(t_pipex *pipex, char **cmd);
char	*ft_strchr_p(const char *s, int c);
char	*ft_strdup_p(const char *s);
char	*access_check(t_pipex *pipex, char **cmd);
void	find_cmd_path(t_pipex *pipex, int num, char **argv);

#endif

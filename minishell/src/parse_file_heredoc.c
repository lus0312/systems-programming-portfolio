#include "minishell.h"

static int	ft_heredoc(int size, char **temp)
{
	int		idx;
	int		rb;
	char	buff[1];

	idx = 0;
	rb = 1;
	write(0, "> ", 2);
	while (rb)
	{
		if (idx == size)
			if (!ft_realloc(temp, &size))
				return (0);
		rb = read(STDIN_FILENO, buff, 1);
		if (rb <= 0)
		{
			free(*temp);
			*temp = NULL;
			break ;
		}
		(*temp)[idx++] = buff[0];
		(*temp)[idx] = '\0';
		if (buff[0] == '\n')
			break ;
	}
	return (1);
}

static void	ft_heredoc_exit(t_data *data, t_list **div, char *eof)
{
	ft_lstclear(div);
	free(eof);
	ft_lstclear(data->input);
	ft_lstclear(data->env);
	exit(EXIT_SUCCESS);
}

static void	ft_parse_heredoc(t_data *data, t_list **div, \
		int fd[2], char *eof)
{
	char	*temp;

	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	temp = NULL;
	if (signal(SIGINT, SIG_DFL) == SIG_ERR || \
	signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		ft_heredoc_exit(data, div, eof);
	while (ft_heredoc(0, &temp) && temp)
	{
		if (ft_is_delimiter(temp, eof))
		{
			g_sig_status = -1;
			free(temp);
			break ;
		}
		ft_heredoc_write(data, temp);
		free(temp);
		temp = NULL;
	}
	ft_heredoc_exit(data, div, eof);
}

int	ft_redirect_heredoc(t_data *data, t_list **div, char *eof)
{
	int		fd[2];
	pid_t	pid;

	g_sig_status = 1;
	if ((signal(SIGINT, ft_signal_int_handler) == SIG_ERR) \
	|| (signal(SIGQUIT, ft_signal_quit_handler) == SIG_ERR))
		return (-1);
	if (pipe(fd))
		return (ft_err_rtn("msh: Error: fail pipe", 1));
	pid = fork();
	if (pid < 0)
		return (ft_err_rtn("msh: Error: fail fork", 1));
	else if (pid == 0)
		ft_parse_heredoc(data, div, fd, eof);
	else
	{
		waitpid(pid, NULL, 0);
		if (g_sig_status == -1)
			return (close(fd[0]), close(fd[1]), -1);
		return (close(fd[1]), fd[0]);
	}
	return (-1);
}

#include "minishell.h"

static void	ft_echo_check_flag(char **cmd, int *i, int *flag)
{
	int	j;

	j = 1;
	if (!cmd[*i])
		return ;
	if (*i == 1 && cmd[*i][0] == '-')
	{
		while (cmd[*i][j] != '\0' && cmd[*i][j] == 'n')
			j++;
		if (cmd[*i][j] == '\0')
		{
			*flag = 1;
			*i += 1;
			if (!cmd[*i])
				return ;
			printf("%s", cmd[*i]);
		}
		else
			printf("%s", cmd[*i]);
		*i += 1;
	}
	else
		printf("%s", cmd[(*i)++]);
}

static void	ft_print_echo(t_data *data)
{
	int		i;
	int		flag;
	char	**cmd;

	i = 1;
	flag = 0;
	cmd = data->cmd;
	ft_echo_check_flag(cmd, &i, &flag);
	while (cmd[i] != NULL)
	{
		printf(" %s", cmd[i]);
		i++;
	}
	if (!flag)
		printf("\n");
}

static void	ft_write_echo_result(t_data *data)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		ft_prt_err(data, "msh: Error: fail fork\n", 1);
	else if (pid == 0)
	{
		dup2((data->now)->fd, STDOUT_FILENO);
		close((data->now)->fd);
		(data->now)->fd = 0;
		ft_print_echo(data);
		ft_excute_builtin_exit(data, 0, 0);
	}
	else
	{
		close((data->now)->fd);
		(data->now)->fd = 0;
		waitpid(pid, NULL, 0);
	}
}

void	ft_builtin_echo(t_data *data)
{
	int		redir;

	redir = (data->now)->redir;
	if (redir == REDIR_NONE || redir == REDIR_EXIST)
		ft_print_echo(data);
	else
	{
		if (redir == REDIR_READ || redir == REDIR_HERE)
		{
			close((data->now)->fd);
			(data->now)->fd = 0;
			ft_print_echo(data);
		}
		else
			ft_write_echo_result(data);
	}
}

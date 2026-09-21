#include "minishell.h"

static void	ft_print_pwd(t_data *data)
{
	t_list	*temp;

	temp = *(data->env);
	while (temp)
	{
		if (!ft_strncmp(temp->data, "PWD=", 4))
		{
			printf("%s\n", &(temp->data)[4]);
			break ;
		}
		temp = temp->next;
	}
}

static void	ft_write_pwd_result(t_data *data)
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
		ft_print_pwd(data);
		ft_excute_builtin_exit(data, 0, 0);
	}
	else
	{
		close((data->now)->fd);
		(data->now)->fd = 0;
		waitpid(pid, NULL, 0);
	}
}

void	ft_builtin_pwd(t_data *data)
{
	int		redir;

	redir = (data->now)->redir;
	if (redir == REDIR_NONE || redir == REDIR_EXIST)
		ft_print_pwd(data);
	else
	{
		if (redir == REDIR_READ || redir == REDIR_HERE)
		{
			close((data->now)->fd);
			(data->now)->fd = 0;
			ft_print_pwd(data);
		}
		else
			ft_write_pwd_result(data);
	}
}

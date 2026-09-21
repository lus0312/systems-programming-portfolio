#include "minishell.h"

static void	ft_print_env(t_list *env)
{
	int		i;
	int		exist;
	char	*temp;

	while (env)
	{
		i = 0;
		exist = 0;
		temp = env->data;
		while (temp[i] != '\0')
		{
			if (env->role == ENV && temp[i] == '=')
			{
				exist = 1;
				break ;
			}
			i++;
		}
		if (exist)
			printf("%s\n", temp);
		env = env->next;
	}
}

static void	ft_write_env_result(t_data *data, t_list *env)
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
		ft_print_env(env);
		ft_excute_builtin_exit(data, 0, 0);
	}
	else
	{
		close((data->now)->fd);
		(data->now)->fd = 0;
		waitpid(pid, NULL, 0);
	}
}

static void	ft_env_error(t_data *data)
{
	ft_close_fd(data);
	write(STDERR_FILENO, "msh: env: '", 11);
	write(STDERR_FILENO, (data->cmd)[1], ft_strlen((data->cmd)[1]));
	write(STDERR_FILENO, "': No such file or directory\n", 29);
	data->status = 127;
}

void	ft_builtin_env(t_data *data)
{
	int		redir;
	t_list	*tenv;

	tenv = *(data->env);
	redir = (data->now)->redir;
	if ((data->cmd)[1])
		return (ft_env_error(data));
	if (redir == REDIR_NONE || redir == REDIR_EXIST)
		ft_print_env(tenv);
	else
	{
		if (redir == REDIR_READ || redir == REDIR_HERE)
		{
			close((data->now)->fd);
			(data->now)->fd = 0;
			ft_print_env(tenv);
		}
		else
			ft_write_env_result(data, tenv);
	}
}

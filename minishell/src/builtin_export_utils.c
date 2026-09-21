#include "minishell.h"

static void	ft_print_export(t_list **env)
{
	int		i;
	char	*temp;
	t_list	*tenv;

	tenv = *env;
	while (tenv)
	{
		i = 0;
		temp = tenv->data;
		if (ft_strncmp(temp, "_=", 2) && tenv->role == ENV)
		{
			printf("declare -x ");
			while (temp[i] != '\0' && temp[i] != '=')
				printf("%c", temp[i++]);
			if (temp[i] == '\0')
				printf("\n");
			else if (temp[i + 1] == '\0')
				printf("=\"\"\n");
			else
				printf("=\"%s\"\n", &temp[i + 1]);
		}
		tenv = tenv->next;
	}
}

static void	ft_write_export_result(t_data *data)
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
		ft_print_export(data->env);
		ft_excute_builtin_exit(data, 0, 0);
	}
	else
	{
		close((data->now)->fd);
		(data->now)->fd = 0;
		waitpid(pid, NULL, 0);
	}
}

void	ft_export_result(t_data *data)
{
	int	redir;

	redir = (data->now)->redir;
	if (redir == REDIR_NONE || redir == REDIR_EXIST)
		ft_print_export(data->env);
	else
	{
		if (redir == REDIR_READ || redir == REDIR_HERE)
		{
			close((data->now)->fd);
			(data->now)->fd = 0;
			ft_print_export(data->env);
		}
		else
			ft_write_export_result(data);
	}
}

int	ft_canexport(char *s)
{
	int	i;

	if (!((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z') \
	|| s[0] == '_'))
		return (0);
	i = 1;
	while (s[i] != '\0' && s[i] != '=')
	{
		if (!ft_isenv(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_export_invalid(t_data *data, char *str)
{
	write(STDERR_FILENO, "msh: export: `", 14);
	write(STDERR_FILENO, str, ft_strlen(str));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
	data->status = 1;
}

#include "minishell.h"

static char	*ft_handle_special_cd(char *str, t_list *env)
{
	char	*rtn;

	rtn = NULL;
	if (!str || (str && str[0] == '~'))
	{
		rtn = ft_get_value("HOME", env, 0);
		if (!ft_strncmp(rtn, ".", 1))
			ft_prt_err(NULL, "msh: cd: HOME not set\n", 0);
	}
	else if (str[0] == '-')
	{
		rtn = ft_get_value("OLDPWD", env, 0);
		if (!ft_strlen(rtn))
			ft_prt_err(NULL, "msh: cd: OLDPWD not set\n", 0);
	}
	if (!rtn)
		return (NULL);
	if (ft_strlen(rtn) == 0)
		rtn = ft_strdup(str);
	if (!rtn)
		return (NULL);
	return (rtn);
}

char	*ft_handle_parameter(char *str, t_list *env, int *prt)
{
	char	*rtn;

	rtn = NULL;
	if (!str || ((str[0] == '~' || str[0] == '-') && str[1] == '\0'))
	{
		rtn = ft_handle_special_cd(str, env);
		if (!ft_strlen(rtn) && str[0] == '-')
			*prt = 1;
	}
	else
		rtn = ft_strdup(str);
	if (!rtn)
	{
		printf("msh: Error!: fail to change path\n");
		return (NULL);
	}
	return (rtn);
}

static void	ft_write_cd_result(t_data *data, char *pwd)
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
		printf("%s\n", pwd);
		ft_excute_builtin_exit(data, 0, 0);
	}
	else
	{
		close((data->now)->fd);
		(data->now)->fd = 0;
		waitpid(pid, NULL, 0);
	}
}

void	ft_cd_result(t_data *data, char *pwd)
{
	int	redir;

	redir = (data->now)->redir;
	if (redir == REDIR_NONE || redir == REDIR_EXIST)
		printf("%s\n", pwd);
	else
	{
		if (redir == REDIR_READ || redir == REDIR_HERE)
		{
			close((data->now)->fd);
			(data->now)->fd = 0;
			printf("%s\n", pwd);
		}
		else
			ft_write_cd_result(data, pwd);
	}
}

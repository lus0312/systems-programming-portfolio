#include "minishell.h"

static void	ft_change_env_pwd(char *pwd, t_list **env)
{
	char	*temp;
	t_list	*tenv;

	tenv = *env;
	while (tenv)
	{
		temp = tenv->data;
		if (!ft_strncmp(temp, "PWD=", 4))
		{
			tenv->data = pwd;
			free(temp);
			break ;
		}
		tenv = tenv->next;
	}
}

static int	ft_handle_env_pwd(char *cwd, t_list **env)
{
	char	*pwd;

	pwd = ft_straddjoin("PWD=", cwd, 0);
	if (!pwd)
		return (1);
	ft_change_env_pwd(pwd, env);
	return (0);
}

static int	ft_handle_env_oldpwd(char *owd, t_list **env)
{
	char	*temp;
	char	*oldpwd;
	t_list	*tenv;

	tenv = *env;
	oldpwd = ft_straddjoin("OLDPWD=", owd, 0);
	if (!oldpwd)
		return (1);
	while (tenv)
	{
		temp = tenv->data;
		if (!ft_strncmp(temp, "OLDPWD", 6) && \
		(temp[6] == '\0' || temp[6] == '='))
		{
			tenv->data = oldpwd;
			free(temp);
			break ;
		}
		tenv = tenv->next;
	}
	return (0);
}

static int	ft_excute_builtin_cd(char *param, t_list **env)
{
	char	*owd;
	char	*cwd;

	owd = getcwd(NULL, 0);
	if (!owd)
		owd = ft_get_value("PWD", *env, 0);
	if (!owd)
		return (ft_err_rtn("msh: Error!: fail to get OLDPWD", 1));
	if (chdir(param) && ft_strlen(param))
	{
		write(STDERR_FILENO, "msh: cd: ", 9);
		perror(param);
		return (free(owd), 1);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (free(owd), ft_err_rtn("msh: Error!: fail to get PWD", 1));
	if (ft_handle_env_pwd(cwd, env) || ft_handle_env_oldpwd(owd, env))
	{
		free(owd);
		free(cwd);
		return (ft_err_rtn("msh: Error!: fail to change PWD", 1));
	}
	return (free(owd), free(cwd), 0);
}

void	ft_builtin_cd(t_data *data)
{
	int		i;
	int		prt;
	char	*temp;

	i = 0;
	prt = 0;
	temp = NULL;
	while ((data->cmd)[i] != NULL)
		i++;
	if (i > 2)
		ft_prt_err(data, "msh: cd: Too many arguments\n", 1);
	else
	{
		temp = ft_handle_parameter((data->cmd)[1], *(data->env), &prt);
		if (!temp)
			ft_prt_err(data, "msh: cd: fail to get path\n", 1);
	}
	if (temp && ft_excute_builtin_cd(temp, data->env))
		data->status = 1;
	if (prt)
		ft_cd_result(data, temp);
	free(temp);
}

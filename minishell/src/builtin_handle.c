#include "minishell.h"

int	ft_is_builtin(char *cmd)
{
	if (!ft_strncmp(cmd, "echo", 4) && ft_strlen(cmd) == 4)
		return (BUILTIN_ECHO);
	else if (!ft_strncmp(cmd, "cd", 2) && ft_strlen(cmd) == 2)
		return (BUILTIN_CD);
	else if (!ft_strncmp(cmd, "pwd", 3) && ft_strlen(cmd) == 3)
		return (BUILTIN_PWD);
	else if (!ft_strncmp(cmd, "export", 6) && ft_strlen(cmd) == 6)
		return (BUILTIN_EXPORT);
	else if (!ft_strncmp(cmd, "unset", 5) && ft_strlen(cmd) == 5)
		return (BUILTIN_UNSET);
	else if (!ft_strncmp(cmd, "env", 3) && ft_strlen(cmd) == 3)
		return (BUILTIN_ENV);
	else if (!ft_strncmp(cmd, "exit", 4) && ft_strlen(cmd) == 4)
		return (BUILTIN_EXIT);
	else
		return (0);
}

void	ft_builtin_excute(int builtin, t_data *data)
{
	data->status = 0;
	if (builtin == BUILTIN_ECHO)
		ft_builtin_echo(data);
	else if (builtin == BUILTIN_PWD)
		ft_builtin_pwd(data);
	else if (builtin == BUILTIN_CD)
		ft_builtin_cd(data);
	else if (builtin == BUILTIN_EXPORT)
		ft_builtin_export(data);
	else if (builtin == BUILTIN_UNSET)
		ft_builtin_unset(data);
	else if (builtin == BUILTIN_ENV)
		ft_builtin_env(data);
	else if (builtin == BUILTIN_EXIT)
		ft_builtin_exit(data);
}

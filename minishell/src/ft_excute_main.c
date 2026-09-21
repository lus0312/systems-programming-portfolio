#include "minishell.h"

static void	ft_err_excute(t_data *data, int status, char **needfree, \
		char *msg)
{
	if (!msg && status == 127 && ft_ispath((data->cmd)[0]))
		msg = "No such file or directory";
	else if (!msg && status == 127)
		msg = "command not found";
	else if (!msg && status == 126)
		msg = "Permission denied";
	if (msg)
	{
		write(STDERR_FILENO, "msh: ", 5);
		write(STDERR_FILENO, (data->cmd)[0], ft_strlen((data->cmd)[0]));
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, msg, ft_strlen(msg));
		write(STDERR_FILENO, "\n", 1);
	}
	if (needfree && *needfree)
		free(*needfree);
	ft_excute_builtin_exit(data, status, 0);
}

static char	*ft_handle_excute_path(t_data *data)
{
	char	*env;
	char	*path;

	if (ft_ispath((data->cmd)[0]))
		path = ft_handle_path((data->cmd)[0], 0);
	else
	{
		env = ft_get_env_path(data->env);
		if (!env)
			ft_err_excute(data, 127, NULL, NULL);
		path = ft_get_path(data, (data->cmd)[0], env);
	}
	if (!path)
		return (NULL);
	if (access(path, F_OK) < 0)
		ft_err_excute(data, 127, NULL, NULL);
	return (path);
}

static void	ft_do_execve(t_data *data, char *path)
{
	char	**envp;

	envp = ft_lsttodata(*(data->env), 1);
	if (!envp)
		ft_err_excute(data, 1, &path, NULL);
	ft_data_clear(NULL, data->input, data->env);
	execve(path, data->cmd, envp);
	ft_free_str(envp);
	ft_err_excute(data, 126, &path, strerror(errno));
}

void	ft_origin_excute(t_data *data)
{
	char		*path;
	struct stat	st;

	if (!data->cmd || !data->cmd[0])
		ft_excute_builtin_exit(data, 0, 0);
	if (!data->cmd[0][0])
		ft_err_excute(data, 127, NULL, NULL);
	path = ft_handle_excute_path(data);
	if (!path)
		ft_err_excute(data, 127, NULL, NULL);
	if (!stat(path, &st) && S_ISDIR(st.st_mode))
		ft_err_excute(data, 126, &path, "Is a directory");
	if (access(path, X_OK) < 0)
		ft_err_excute(data, 126, &path, NULL);
	ft_do_execve(data, path);
}

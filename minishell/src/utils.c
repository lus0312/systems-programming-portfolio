#include "minishell.h"

void	ft_free_str(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		if (arr[i])
			free(arr[i]);
		i++;
	}
	free(arr);
}

void	ft_free_int(int **arr, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (arr[i])
			free(arr[i]);
		i++;
	}
	free(arr);
}

void	ft_data_clear(char **cmd, t_list **input, t_list **env)
{
	if (cmd)
	{
		ft_free_str(cmd);
		cmd = NULL;
	}
	if (input)
	{
		ft_lstclear(input);
		input = NULL;
	}
	if (env)
	{
		ft_lstclear(env);
		env = NULL;
	}
}

void	ft_prt_err(t_data *data, char *str, int status)
{
	if (data)
		data->status = status;
	write(STDERR_FILENO, str, ft_strlen(str));
}

int	ft_err_rtn(char *str, int value)
{
	write(STDERR_FILENO, str, ft_strlen(str));
	return (value);
}

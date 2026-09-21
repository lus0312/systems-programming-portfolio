#include "minishell.h"

void	ft_excute_builtin_exit(t_data *data, int num, int isexit)
{
	unsigned char	value;

	if (data)
	{
		if (data->cmd)
			ft_free_str(data->cmd);
		if (data->input)
			ft_lstclear(data->input);
		if (data->env)
			ft_lstclear(data->env);
	}
	rl_clear_history();
	if (isexit)
		printf("exit\n");
	value = (unsigned char)num;
	exit(value);
}

static void	ft_handle_exit_parameter(t_data *data, \
		char *snum, int *num, int *err)
{
	ft_lstclear(data->input);
	ft_lstclear(data->env);
	*num = ft_atoi(snum, err);
	if (*err)
	{
		write(STDERR_FILENO, "exit\nmsh: exit: ", 16);
		write(STDERR_FILENO, snum, ft_strlen(snum));
		write(STDERR_FILENO, ": numeric argument required\n", 28);
		ft_free_str(data->cmd);
		exit(2);
	}
	else
	{
		printf("exit\n");
		ft_free_str(data->cmd);
		exit(*num);
	}
}

static void	ft_handle_exit_many_parameter(t_data *data, \
		char *snum, int *num, int *err)
{
	*num = ft_atoi(snum, err);
	if (*err)
	{
		write(STDERR_FILENO, "exit\nmsh: exit: ", 16);
		write(STDERR_FILENO, snum, ft_strlen(snum));
		write(STDERR_FILENO, ": numeric argument required\n", 28);
		data->status = 1;
		ft_free_str(data->cmd);
		ft_lstclear(data->input);
		ft_lstclear(data->env);
		exit(2);
	}
	else
		ft_prt_err(data, "exit\nmsh: exit: too many arguments\n", 1);
}

void	ft_builtin_exit(t_data *data)
{
	int		i;
	int		err;
	int		num;
	char	**cmd;

	i = 0;
	num = 0;
	err = 0;
	ft_close_fd(data);
	cmd = data->cmd;
	while (cmd[i] != NULL)
		i++;
	if (i > 2)
	{
		ft_handle_exit_many_parameter(data, cmd[1], &num, &err);
		if (err)
			return ;
	}
	else if (i == 2)
		ft_handle_exit_parameter(data, cmd[1], &num, &err);
	else
		ft_excute_builtin_exit(data, data->status, 1);
}

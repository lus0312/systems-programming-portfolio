#include "minishell.h"

static void	ft_set_origin_excute(t_data *data)
{
	pid_t	pid;
	int		raw;

	pid = fork();
	if (pid < 0)
		return (ft_prt_err(data, "msh: Error!: fork fail", 1));
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		ft_handle_fd_origin(data);
		ft_origin_excute(data);
	}
	else
	{
		g_sig_status = 1;
		if ((signal(SIGINT, ft_signal_int_handler) == SIG_ERR) \
		|| (signal(SIGQUIT, ft_signal_quit_handler) == SIG_ERR))
			return ;
		raw = 0;
		waitpid(pid, &raw, 0);
		ft_set_exit_status(&data->status, raw);
	}
}

static int	ft_excute_normal(t_data *data, t_list *temp)
{
	int		builtin;

	builtin = 0;
	data->cmd = NULL;
	data->cmd = ft_parse_data(data, temp->data);
	if (!(data->cmd))
		return (1);
	if (!(data->cmd)[0])
		return (ft_close_fd(data), ft_free_str(data->cmd), 0);
	builtin = ft_is_builtin((data->cmd)[0]);
	if (builtin)
		ft_builtin_excute(builtin, data);
	else
		ft_set_origin_excute(data);
	ft_free_str(data->cmd);
	return (0);
}

static int	ft_fail_status(void)
{
	if (g_sig_status == -1)
		return (130);
	return (1);
}

static void	ft_excute_input(t_data *data)
{
	t_list	*temp;

	temp = *(data->input);
	if (!ft_strlen(temp->data))
		return (ft_prt_err(data, "msh: Error!: Command '' not found\n", 1));
	while (temp)
	{
		data->now = temp;
		if (temp->role == PCMD)
		{
			temp = ft_excute_pipe(data, &temp);
			if (temp && temp->role == PCMD)
				return (ft_prt_err(data, "", 1));
		}
		if (temp && temp->role == CMD)
			if (ft_excute_normal(data, temp))
				return (ft_prt_err(data, "", ft_fail_status()));
		if (temp)
			temp = temp->next;
	}
}

int	ft_run_minishell(char **line, t_list **env, int *status)
{
	t_list		*input;
	t_data		data;

	if (ft_check_right_cmd(*line, status))
		return (0);
	input = ft_parse_input(line);
	if (!input)
		return (0);
	data.status = *status;
	data.input = &input;
	data.env = env;
	ft_excute_input(&data);
	*status = data.status;
	ft_lstclear(&input);
	return (0);
}

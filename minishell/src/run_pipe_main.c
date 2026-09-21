#include "minishell.h"

static int	ft_excute_pipe_command(t_data *data, t_list *cmd)
{
	int		builtin;

	builtin = 0;
	data->cmd = NULL;
	data->cmd = ft_parse_data(data, cmd->data);
	if (!(data->cmd))
		return (1);
	if (!(data->cmd)[0])
		ft_excute_builtin_exit(data, 0, 0);
	builtin = ft_is_builtin((data->cmd)[0]);
	if (builtin)
	{
		ft_builtin_excute(builtin, data);
		exit(data->status);
	}
	else
	{
		ft_handle_fd_origin(data);
		ft_origin_excute(data);
	}
	if (data->status < 0)
		ft_prt_err(data, "msh: execve works wrong\n", 1);
	ft_data_clear(data->cmd, data->input, data->env);
	return (0);
}

static void	ft_run_pipe_parent(t_ph *ph, int i)
{
	if (i)
	{
		close(ph->fd[i - 1][0]);
		free((ph->fd)[i - 1]);
		if (i == ph->p_len)
			free(ph->fd);
	}
	if (i != ph->p_len)
		close(ph->fd[i][1]);
}

static int	ft_run_pipe(t_data *data, t_list *cmd, t_ph *ph, int i)
{
	g_sig_status = 1;
	if ((signal(SIGINT, ft_signal_int_handler) == SIG_ERR) \
	|| (signal(SIGQUIT, ft_signal_quit_handler) == SIG_ERR))
		return (-1);
	if (i < ph->p_len)
		pipe(ph->fd[i]);
	ph->pid[i] = fork();
	if (ph->pid[i] < 0)
		return (1);
	else if (!ph->pid[i])
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		ft_handle_child_pipe(ph, i);
		if (ft_excute_pipe_command(data, cmd))
			return (1);
	}
	else
		ft_run_pipe_parent(ph, i);
	return (0);
}

t_list	*ft_excute_pipe(t_data *data, t_list **cmd)
{
	int		i;
	t_ph	ph;

	i = 0;
	ph.p_len = 0;
	ph.pid = NULL;
	if (ft_set_pipe_arg(*cmd, &ph))
		return (*cmd);
	while (i < ph.p_len + 1)
	{
		if (ft_run_pipe(data, *cmd, &ph, i))
			return (ft_free_pipe_arg(&ph, i), *cmd);
		*cmd = (*cmd)->next;
		i++;
	}
	ft_wait_pipe(data, &ph);
	return (*cmd);
}

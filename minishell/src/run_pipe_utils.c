#include "minishell.h"

int	ft_set_pipe_arg(t_list *cmd, t_ph *ph)
{
	int	size;

	while (cmd && cmd->role == PCMD)
	{
		cmd = cmd->next;
		ph->p_len += 1;
	}
	size = ph->p_len;
	ph->fd = malloc(sizeof(int *) * size);
	if (!(ph->fd))
		return (1);
	ph->pid = malloc(sizeof(pid_t) * (size + 1));
	if (!(ph->pid))
		return (free(ph->fd), 1);
	while (size > 0)
	{
		(ph->fd)[size - 1] = malloc(sizeof(int) * 2);
		if (!ph->fd[size - 1])
			return (ft_free_int(ph->fd, ph->p_len), free(ph->pid), 1);
		size--;
	}
	return (0);
}

void	ft_free_pipe_arg(t_ph *ph, int s)
{
	while (s < ph->p_len)
	{
		close((ph->fd)[s][0]);
		close((ph->fd)[s][1]);
		if (s)
		{
			close((ph->fd)[s - 1][0]);
			free((ph->fd)[s - 1]);
		}
		free((ph->fd)[s]);
		s++;
	}
	free(ph->pid);
}

void	ft_handle_child_pipe(t_ph *ph, int end)
{
	if (end == ph->p_len)
	{
		dup2(ph->fd[end - 1][0], STDIN_FILENO);
		close(ph->fd[end - 1][0]);
		free((ph->fd)[end - 1]);
		ft_free_pipe_arg(ph, end);
	}
	else
	{
		close(ph->fd[end][0]);
		if (end)
		{
			dup2(ph->fd[end - 1][0], STDIN_FILENO);
			close(ph->fd[end - 1][0]);
			free((ph->fd)[end - 1]);
		}
		dup2(ph->fd[end][1], STDOUT_FILENO);
		close(ph->fd[end][1]);
		free((ph->fd)[end]);
	}
}

void	ft_set_exit_status(int *status, int raw)
{
	if (WIFEXITED(raw))
		*status = WEXITSTATUS(raw);
	else if (WIFSIGNALED(raw))
	{
		*status = 128 + WTERMSIG(raw);
		if (WTERMSIG(raw) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
}

void	ft_wait_pipe(t_data *data, t_ph *ph)
{
	int	i;
	int	raw;

	i = 0;
	while (i < ph->p_len + 1)
	{
		raw = 0;
		waitpid(ph->pid[i], &raw, 0);
		if (i == ph->p_len)
			ft_set_exit_status(&(data->status), raw);
		i++;
	}
	if (ph->pid)
		free(ph->pid);
}

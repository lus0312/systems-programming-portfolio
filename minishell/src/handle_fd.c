#include "minishell.h"

void	ft_close_fd(t_data *data)
{
	int	redir;

	redir = (data->now)->redir;
	if (redir == REDIR_EXIST || redir == REDIR_NONE)
		return ;
	else
	{
		close((data->now)->fd);
		(data->now)->fd = 0;
	}
}

static void	ft_clear_fd(t_list **lst)
{
	t_list	*temp;

	temp = *lst;
	while (temp)
	{
		if (temp->fd)
			close(temp->fd);
		temp = temp->next;
	}
}

void	ft_handle_fd_origin(t_data *data)
{
	int		redir;
	t_list	*temp;

	temp = data->now;
	redir = temp->redir;
	if (redir == REDIR_NONE)
		return ;
	else
	{
		if (redir == REDIR_EXIST)
			ft_clear_fd(&temp);
		else
		{
			if (redir == REDIR_READ || redir == REDIR_HERE)
				dup2(temp->fd, STDIN_FILENO);
			else if (redir == REDIR_TRUNC || redir == REDIR_APPEND)
				dup2(temp->fd, STDOUT_FILENO);
			close(temp->fd);
			temp->fd = 0;
			ft_clear_fd(&temp);
		}
	}
}

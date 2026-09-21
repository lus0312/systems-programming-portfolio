#include "minishell.h"

void	ft_builtin_unset(t_data *data)
{
	int		i;
	int		len;
	char	**cmd;
	t_list	*tenv;

	i = 1;
	ft_close_fd(data);
	cmd = data->cmd;
	while (cmd[i] != NULL)
	{
		tenv = *(data->env);
		len = ft_strlen(cmd[i]);
		while (tenv)
		{
			if (!ft_strncmp(tenv->data, cmd[i], len) && \
			(tenv->data[len] == '=' || tenv->data[len] == '\0'))
			{
				tenv->role = UNSET;
				break ;
			}
			tenv = tenv->next;
		}
		i++;
	}
}

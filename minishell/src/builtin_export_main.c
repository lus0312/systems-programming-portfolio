#include "minishell.h"

static int	ft_handle_env_status(t_list **lst, char *data)
{
	char	*temp;
	t_list	*tlst;

	tlst = *lst;
	temp = data;
	while (*temp != '\0' && *temp != '=')
		temp++;
	if (tlst->role == ENV && *temp == '\0')
		return (1);
	tlst->role = ENV;
	if (!ft_strncmp(tlst->data, "PWD", 3))
		return (1);
	else
	{
		temp = tlst->data;
		tlst->data = ft_strdup(data);
		if (!tlst->data)
			return (-1);
		free(temp);
		return (1);
	}
	return (0);
}

static int	ft_find_unsetvalue(t_list **env, char *data)
{
	int		len;
	t_list	*temp;

	len = 0;
	while (data[len] != '\0' && data[len] != '=')
		len++;
	temp = *env;
	while (temp)
	{
		if (!ft_strncmp(temp->data, data, len) && \
		(temp->data[len] == '=' || temp->data[len] == '\0'))
			return (ft_handle_env_status(&temp, data));
		temp = temp->next;
	}
	return (0);
}

static t_list	*ft_add_data(t_list **env, char *data)
{
	int		check;
	char	*dup;
	char	*temp;
	t_list	*last;

	check = ft_find_unsetvalue(env, data);
	if (check < 0)
		return (ft_lstclear(env), NULL);
	else if (check)
		return (*env);
	else
	{
		dup = ft_strdup(data);
		if (!dup)
			return (ft_lstclear(env), NULL);
		last = ft_lstlast(*env);
		temp = last->data;
		last->data = dup;
		*env = ft_lstadd_back(env, temp, ENV);
		if (!*env)
			return (NULL);
	}
	return (*env);
}

void	ft_builtin_export(t_data *data)
{
	int		i;
	char	**cmd;

	i = 1;
	cmd = data->cmd;
	while (cmd[i] != NULL)
	{
		if (i == 1)
			ft_close_fd(data);
		if (!ft_canexport(cmd[i]))
			ft_export_invalid(data, cmd[i]);
		else
			*(data->env) = ft_add_data(data->env, cmd[i]);
		if (!*(data->env))
			return (ft_prt_err(data, "msh: Error!: fail to add data", 2));
		i++;
	}
	if (i == 1)
		ft_export_result(data);
}

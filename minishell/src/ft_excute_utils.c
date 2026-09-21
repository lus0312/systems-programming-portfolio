#include "minishell.h"

char	**ft_lsttodata(t_list *lst, int is_env)
{
	int		j;
	char	**rtn;

	rtn = malloc(sizeof(char *) * (ft_lstsize(lst) + 1));
	if (!rtn)
		return (NULL);
	j = 0;
	while (lst)
	{
		if (!is_env || lst->role == ENV)
		{
			rtn[j] = ft_strdup(lst->data);
			if (!rtn[j++])
				return (rtn[j - 1] = NULL, ft_free_str(rtn), NULL);
		}
		lst = lst->next;
	}
	rtn[j] = NULL;
	return (rtn);
}

char	*ft_get_env_path(t_list **env)
{
	t_list	*tenv;

	tenv = *env;
	while (tenv)
	{
		if (!ft_strncmp(tenv->data, "PATH", 4))
		{
			if ((tenv->data)[4] == '=' && tenv->role == ENV)
				return (tenv->data);
			else
				return (NULL);
		}
		tenv = tenv->next;
	}
	return (NULL);
}

char	*ft_get_path(t_data *data, char *cmd, char *env)
{
	int		i;
	char	*filename;
	char	**paths;

	paths = ft_split(env + 5, ':');
	if (!paths)
		ft_excute_builtin_exit(data, EXIT_FAILURE, 0);
	i = 0;
	while (paths[i] != NULL)
	{
		filename = ft_straddjoin(paths[i], cmd, '/');
		if (!filename)
		{
			ft_free_str(paths);
			ft_excute_builtin_exit(data, EXIT_FAILURE, 0);
		}
		if (!access(filename, X_OK))
			return (ft_free_str(paths), filename);
		free(filename);
		i++;
	}
	ft_free_str(paths);
	return (NULL);
}

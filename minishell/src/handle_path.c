#include "minishell.h"

int	ft_ispath(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_wrap_path(char **rtn)
{
	char	*temp;

	temp = *rtn;
	*rtn = ft_straddjoin("/", temp, 0);
	if (!*rtn)
		return (free(temp), NULL);
	free(temp);
	return (*rtn);
}

static char	*ft_combine_path(char **path, char **rtn, int s)
{
	int		i;
	char	*temp;

	*rtn = ft_strdup(path[0]);
	if (!*rtn)
		return (NULL);
	i = 1;
	while (path[i] != NULL)
	{
		temp = *rtn;
		*rtn = ft_straddjoin(temp, path[i], '/');
		if (!*rtn)
			return (free(temp), NULL);
		free(temp);
		i++;
	}
	if (s)
	{
		*rtn = ft_wrap_path(rtn);
		if (!*rtn)
			return (NULL);
	}
	return (*rtn);
}

static void	ft_check_path(char *str, int *s, int *e, int cd)
{
	int	i;

	i = 0;
	if (str[i] == '/')
		*s = 1;
	while (str[i] != '\0')
		i++;
	if (str[i - 1] == '/')
		*e = 1;
	if (cd)
		*e = 0;
}

char	*ft_handle_path(char *str, int cd)
{
	int		i;
	int		s;
	int		e;
	char	*rtn;
	char	**path;

	s = 0;
	e = 0;
	ft_check_path(str, &s, &e, cd);
	rtn = NULL;
	path = ft_split(str, '/');
	if (!path)
		return (NULL);
	i = 0;
	while (path[i] != NULL)
		i++;
	if (!e)
		rtn = ft_combine_path(path, &rtn, s);
	else
		rtn = ft_strdup(str);
	if (!rtn)
		return (ft_free_str(path), NULL);
	ft_free_str(path);
	return (rtn);
}

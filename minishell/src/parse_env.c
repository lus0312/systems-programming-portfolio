#include "minishell.h"

int	ft_isenv(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || \
	(c >= '0' && c <= '9') || c == '_')
		return (1);
	return (0);
}

static char	*ft_get_key(char *token, t_cmd *h)
{
	char	*key;

	h->i = 0;
	while (token[h->i] != '\0')
	{
		if (token[h->i] == '$')
		{
			h->s = h->i;
			h->i += 1;
			while (token[h->i] != '\0' && ft_isenv(token[h->i]))
				h->i += 1;
			if (token[h->i] == '?')
				h->i += 1;
			break ;
		}
		if (token[h->i] != '\0')
			h->i += 1;
	}
	key = ft_strndup(&token[h->s + 1], h->i - h->s - 1);
	if (!key)
		return (NULL);
	return (key);
}

static char	*ft_find_value(char *key, t_list *env)
{
	int		len;
	char	*temp;

	len = ft_strlen(key);
	while (env)
	{
		temp = env->data;
		if (!ft_strncmp(temp, key, len) && temp[len] == '=')
		{
			if (env->role != ENV && \
			(!ft_strncmp("OLDPWD", key, 6) || \
			!ft_strncmp("HOME", key, 4)))
				return (ft_strdup("."));
			else
				return (ft_strdup(&temp[len + 1]));
		}
		env = env->next;
	}
	return (ft_strdup(""));
}

char	*ft_get_value(char *key, t_list *env, int status)
{
	char	*rtn;

	if (g_sig_status == 130)
		status = 130;
	if (key[0] == '?')
		rtn = ft_itoa(status);
	else if (key[0] == '\0')
		rtn = ft_strdup("$");
	else
		rtn = ft_find_value(key, env);
	if (!rtn)
		return (NULL);
	return (rtn);
}

char	*ft_parse_env(char *token, t_list *env, int *status)
{
	t_cmd	h;
	char	*key;
	char	*value;

	key = ft_get_key(token, &h);
	if (!key)
		return (NULL);
	value = ft_get_value(key, env, *status);
	if (!value)
		return (free(key), NULL);
	free(key);
	return (value);
}

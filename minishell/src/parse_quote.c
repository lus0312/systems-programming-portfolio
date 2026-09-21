#include "minishell.h"

static char	*ft_parse_single_quote(char *token)
{
	t_cmd	h;

	h.s = 1;
	h.i = 1;
	while (token[h.i] != '\0' && token[h.i] != '\'')
		h.i += 1;
	h.temp = ft_strndup(&token[h.s], h.i - h.s);
	if (!h.temp)
		return (NULL);
	return (h.temp);
}

static t_list	*ft_parse_quote_env_handler(t_list **q_lst, \
		char *token, t_list *env, t_cmd *h)
{
	char	*value;

	h->i += 1;
	h->temp = NULL;
	while (token[h->i] != '\0' && ft_isenv(token[h->i]))
		h->i += 1;
	if (token[h->i] == '?')
		h->i += 1;
	h->temp = ft_strndup(&token[h->s + 1], h->i - h->s - 1);
	if (!h->temp)
		return (ft_lstclear(q_lst), NULL);
	value = ft_get_value(h->temp, env, h->role);
	if (!value)
		return (ft_lstclear(q_lst), free(h->temp), NULL);
	free(h->temp);
	*q_lst = ft_lstadd_back(q_lst, value, 0);
	if (!*q_lst)
		return (free(h->temp), NULL);
	h->s = h->i;
	return (*q_lst);
}

static t_list	*ft_parse_quote_env(t_list **q_lst, char *token, \
		t_list *env, t_cmd *h)
{
	if (h->i != h->s)
	{
		if (token[h->i] == '\0')
			h->temp = ft_strndup(&token[h->s], h->i - h->s - 1);
		else
			h->temp = ft_strndup(&token[h->s], h->i - h->s);
		if (!(h->temp))
		{
			if (*q_lst)
				ft_lstclear(q_lst);
			return (NULL);
		}
		*q_lst = ft_lstadd_back(q_lst, h->temp, 0);
		if (!*q_lst)
			return (free(h->temp), NULL);
		h->s = h->i;
	}
	if (token[h->i] == '$')
	{
		*q_lst = ft_parse_quote_env_handler(q_lst, token, env, h);
		if (!*q_lst)
			return (NULL);
	}
	return (*q_lst);
}

static char	*ft_parse_double_quote(char *token, t_list *env, int status)
{
	t_cmd	h;
	t_list	*q_lst;

	h.s = 1;
	h.i = 1;
	h.role = status;
	q_lst = NULL;
	while (1)
	{
		if (token[h.i] == '$' || token[h.i] == '\0')
		{
			q_lst = ft_parse_quote_env(&q_lst, token, env, &h);
			if (!q_lst)
				return (NULL);
		}
		if (token[h.i] == '\0')
			break ;
		else
			h.i += 1;
	}
	h.temp = ft_lsttostr(&q_lst);
	if (!h.temp)
		return (ft_lstclear(&q_lst), NULL);
	ft_lstclear(&q_lst);
	return (h.temp);
}

char	*ft_parse_quote(char *token, t_list *env, int status)
{
	char	*rtn;

	rtn = NULL;
	if (token[0] == '\'')
		rtn = ft_parse_single_quote(token);
	else if (token[0] == '"')
		rtn = ft_parse_double_quote(token, env, status);
	if (!rtn)
		return (NULL);
	return (rtn);
}

#include "minishell.h"

int	ft_has_quote(char *str)
{
	while (*str != '\0')
	{
		if (*str == '\'' || *str == '"')
			return (1);
		str++;
	}
	return (0);
}

static t_list	*ft_divide_literal(t_list **div, char *token, t_cmd *h)
{
	char	*temp;

	while (token[h->i] != '\0' && token[h->i] != '$' && \
	token[h->i] != '\'' && token[h->i] != '"')
		h->i += 1;
	temp = ft_strndup(&token[h->s], h->i - h->s);
	if (!temp)
		return (NULL);
	return (ft_lstadd_back(div, temp, 0));
}

t_list	*ft_divide_redirect_token(char *token, t_list **div)
{
	char	c;
	t_cmd	h;

	c = token[0];
	h.i = 0;
	while (token[h.i] != '\0' && (token[h.i] == c || token[h.i] == ' '))
		h.i += 1;
	while (token[h.i] != '\0')
	{
		h.s = h.i;
		if (token[h.i] == '$')
			*div = ft_divide_env(div, token, &h);
		else if (token[h.i] == '\'' || token[h.i] == '"')
			*div = ft_divide_quote(div, token, &h);
		else
			*div = ft_divide_literal(div, token, &h);
		if (!(*div))
			return (NULL);
	}
	return (*div);
}

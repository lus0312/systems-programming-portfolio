#include "minishell.h"

int	ft_need_divide(char c)
{
	if (c == '\'' || c == '"' || c == '<' || c == '>' || \
	c == '$' || c == ' ' || c == '\0')
		return (1);
	return (0);
}

t_list	*ft_divide_quote(t_list **div, char *cmd, t_cmd *h)
{
	char	*temp;

	h->i += 1;
	while (cmd[h->i] != '\0' && cmd[h->i] != cmd[h->s])
		h->i += 1;
	temp = ft_strndup(&cmd[h->s], h->i - h->s + 1);
	if (!temp)
		return (NULL);
	*div = ft_lstadd_back(div, temp, 0);
	if (!*div)
		return (NULL);
	if (cmd[h->i] != '\0')
		h->i += 1;
	return (*div);
}

t_list	*ft_divide_redirect(t_list **div, char *cmd, t_cmd *h)
{
	char	*temp;

	while (cmd[h->i] != '\0' && cmd[h->i] == cmd[h->s])
		h->i += 1;
	while (cmd[h->i] != '\0' && cmd[h->i] == ' ')
		h->i += 1;
	while (cmd[h->i] != '\0' && cmd[h->i] != ' ')
		h->i += 1;
	temp = ft_strndup(&cmd[h->s], h->i - h->s);
	if (!temp)
		return (NULL);
	*div = ft_lstadd_back(div, temp, 0);
	if (!*div)
		return (NULL);
	return (*div);
}

t_list	*ft_divide_env(t_list **div, char *cmd, t_cmd *h)
{
	char	*temp;

	if (cmd[h->i] == '$' && cmd[h->i + 1] == '?')
	{
		temp = ft_strdup("$?");
		h->i += 2;
	}
	else
	{
		h->i += 1;
		while (cmd[h->i] != '\0' && ft_isenv(cmd[h->i]))
			h->i += 1;
		temp = ft_strndup(&cmd[h->s], h->i - h->s);
		if (!temp)
			return (NULL);
	}
	*div = ft_lstadd_back(div, temp, 0);
	if (!*div)
		return (NULL);
	return (*div);
}

t_list	*ft_divide_special_block(char *cmd, t_list **div, t_cmd *h)
{
	h->s = h->i;
	if (cmd[h->i] == '\'' || cmd[h->i] == '"')
		*div = ft_divide_quote(div, cmd, h);
	else if (cmd[h->i] == '<' || cmd[h->i] == '>')
		*div = ft_divide_redirect(div, cmd, h);
	else
		*div = ft_divide_env(div, cmd, h);
	if (!*div)
		return (NULL);
	return (*div);
}

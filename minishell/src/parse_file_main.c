#include "minishell.h"

static int	ft_get_role(char *redirect)
{
	int		role;
	int		i;

	role = 0;
	i = 0;
	if (redirect[i] == '>')
	{
		role = REDIR_TRUNC;
		if (!ft_strncmp(&redirect[i], ">>", 2))
			role = REDIR_APPEND;
	}
	else
	{
		role = REDIR_READ;
		if (!ft_strncmp(&redirect[i], "<<", 2))
			role = REDIR_HERE;
	}
	return (role);
}

static t_list	*ft_parse_redirect_token(t_list **div, \
		t_list *env, int *status)
{
	char	*token;
	t_list	*temp;

	temp = *div;
	while (temp)
	{
		token = temp->data;
		if (token[0] == '\'' || token[0] == '"')
			temp->data = ft_parse_quote(token, env, *status);
		else if (token[0] == '$')
			temp->data = ft_parse_env(token, env, status);
		if (!temp->data)
			return (NULL);
		temp = temp->next;
	}
	return (*div);
}

static char	*ft_parse_file(char *token, t_list *env, int *status)
{
	char	*filename;
	t_list	*div;

	div = NULL;
	div = ft_divide_redirect_token(token, &div);
	if (!div)
		return (NULL);
	div = ft_parse_redirect_token(&div, env, status);
	if (!div)
		return (ft_lstclear(&div), NULL);
	filename = ft_lsttostr(&div);
	if (!filename)
		return (ft_lstclear(&div), NULL);
	ft_lstclear(&div);
	return (filename);
}

int	ft_parse_redirect(t_data *data, t_list **div, \
	char *token, int *status)
{
	int		i;
	int		role;
	char	*redirect;

	i = 0;
	while (token[i] != '\0' && (token[i] == token[0] || token[i] == ' '))
		i++;
	redirect = ft_parse_file(token, *(data->env), status);
	if (!redirect)
		return (1);
	role = ft_get_role(token);
	data->quoted = ft_has_quote(&token[i]);
	if (role == REDIR_READ || role == REDIR_HERE)
	{
		if (ft_parse_file_in(data, div, redirect, role))
			return (free(redirect), 1);
	}
	else if (role == REDIR_TRUNC || role == REDIR_APPEND)
	{
		if (ft_parse_file_out(data->input, redirect, role))
			return (free(redirect), 1);
	}
	return (free(redirect), 0);
}

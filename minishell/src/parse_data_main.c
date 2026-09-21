#include "minishell.h"

static t_list	*ft_divide_block(char *cmd, t_list **div, t_cmd *h)
{
	char	*temp;

	h->s = h->i;
	while (!ft_need_divide(cmd[h->i]))
		h->i += 1;
	temp = ft_strndup(&cmd[h->s], h->i - h->s);
	if (!temp)
		return (NULL);
	*div = ft_lstadd_back(div, temp, 0);
	if (!*div)
		return (NULL);
	return (*div);
}

static t_list	*ft_divide_cmd(char *cmd, t_list **div)
{
	t_cmd	h;
	int		glue;

	h.s = 0;
	h.i = 0;
	while (cmd[h.i] != '\0')
	{
		glue = h.i > 0 && cmd[h.i - 1] != ' ';
		if (cmd[h.i] != ' ' && ft_need_divide(cmd[h.i]))
			*div = ft_divide_special_block(cmd, div, &h);
		else if (cmd[h.i] != ' ')
			*div = ft_divide_block(cmd, div, &h);
		else
		{
			h.i += 1;
			continue ;
		}
		if (!*div)
			return (NULL);
		if (glue)
			ft_lstlast(*div)->role = GLUE;
	}
	return (*div);
}

static int	ft_handle_parse_redirection(t_data *data, \
		t_list **div, t_list **before, t_list **temp)
{
	char	*token;

	token = (*temp)->data;
	if (ft_parse_redirect(data, div, token, &(data->status)))
		return (1);
	if (*before)
		(*before)->next = (*temp)->next;
	else
		*div = (*temp)->next;
	free((*temp)->data);
	free(*temp);
	*temp = *before;
	return (0);
}

static int	ft_parse_token(t_data *data, t_list **div, int *status)
{
	char	*token;
	t_list	*temp;
	t_list	*before;

	temp = *div;
	before = NULL;
	while (temp)
	{
		token = temp->data;
		temp->data = ft_expand_token(data, token, status);
		if (!temp->data)
			return (*status = 1, temp->data = token, 1);
		if (token[0] == '<' || token[0] == '>')
			if (ft_handle_parse_redirection(data, div, &before, &temp))
				return (*status = 1, temp->data = token, 1);
		free(token);
		if (!temp)
			temp = *div;
		else
		{
			before = temp;
			temp = temp->next;
		}
	}
	return (0);
}

char	**ft_parse_data(t_data *data, char *cmd)
{
	char	**rtn;
	t_list	*div;

	div = NULL;
	div = ft_divide_cmd(cmd, &div);
	if (!div)
		return (NULL);
	if (ft_parse_token(data, &div, &(data->status)))
		return (ft_lstclear(&div), NULL);
	if (ft_merge_glue(div))
		return (ft_lstclear(&div), NULL);
	rtn = ft_lsttodata(div, 0);
	if (!rtn)
		return (ft_lstclear(&div), NULL);
	ft_lstclear(&div);
	return (rtn);
}

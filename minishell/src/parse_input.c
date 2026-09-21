#include "minishell.h"

static int	ft_isdivider(char c)
{
	if (c == '|' || c == '\n' || c == '\0')
		return (1);
	return (0);
}

static void	ft_parse_role(char *line, t_cmd *h, t_list **rtn)
{
	t_list	*temp;

	if (line[h->i] == '|')
	{
		if (line[h->i + 1] != '|')
		{
			h->role = PCMD;
			temp = ft_lstlast(*rtn);
			temp->role = PCMD;
			h->i += 1;
		}
		else if (line[h->i + 1] == '|')
			h->role = CMD;
	}
	else
	{
		temp = ft_lstlast(*rtn);
		temp->role = CMD;
		h->role = CMD;
	}
	h->s = h->i;
}

static t_list	*ft_parse_input_helper(t_list **rtn, char *line, t_cmd *h)
{
	if (line[h->i] != '\0' && h->i == 0)
		h->i += 1;
	while (line[h->i] != '\0' && !ft_isdivider(line[h->i]))
		h->i += 1;
	h->temp = ft_strndup(&line[h->s], h->i - h->s);
	if (!h->temp)
		return (NULL);
	*rtn = ft_lstadd_back(rtn, h->temp, h->role);
	if (!rtn)
		return (free(h->temp), NULL);
	ft_parse_role(line, h, rtn);
	return (*rtn);
}

t_list	*ft_parse_loop(char **str, t_cmd *h, t_list *rtn)
{
	char	*line;

	line = *str;
	while (1)
	{
		if (line[h->i] == '"' || line[h->i] == '\'')
			ft_pass_quotation(line, &h->i);
		if (ft_isdivider(line[h->i]))
		{
			if (line[h->i] == '\n')
			{
				h->i++;
				continue ;
			}
			rtn = ft_parse_input_helper(&rtn, line, h);
			if (!rtn || line[h->i] == '\0')
				break ;
		}
		h->i++;
	}
	return (rtn);
}

t_list	*ft_parse_input(char **line)
{
	t_cmd	h;
	t_list	*rtn;

	h.i = 0;
	h.s = 0;
	h.role = CMD;
	rtn = NULL;
	rtn = ft_parse_loop(line, &h, rtn);
	if (!rtn)
		return (NULL);
	free(*line);
	return (rtn);
}

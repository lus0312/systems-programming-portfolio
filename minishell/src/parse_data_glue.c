#include "minishell.h"

char	*ft_expand_token(t_data *data, char *token, int *status)
{
	if (token[0] == '\'' || token[0] == '"')
		return (ft_parse_quote(token, *(data->env), *status));
	if (token[0] == '$')
		return (ft_parse_env(token, *(data->env), status));
	return (ft_strdup(token));
}

int	ft_merge_glue(t_list *div)
{
	t_list	*next;
	char	*joined;

	while (div && div->next)
	{
		next = div->next;
		if (next->role != GLUE)
		{
			div = next;
			continue ;
		}
		joined = ft_straddjoin(div->data, next->data, 0);
		if (!joined)
			return (1);
		free(div->data);
		div->data = joined;
		div->next = next->next;
		free(next->data);
		free(next);
	}
	return (0);
}

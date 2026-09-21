#include "minishell.h"

void	ft_pass_quotation(char *line, int *i)
{
	char	c;

	c = line[*i];
	*i += 1;
	while (line[*i] != '\0' && line[*i] != c)
		*i += 1;
}

char	**ft_dup_str(char *str)
{
	char	**rtn;

	rtn = malloc(sizeof(char *) * 2);
	if (!rtn)
		return (NULL);
	rtn[0] = ft_strdup(str);
	if (!rtn[0])
		return (NULL);
	rtn[1] = NULL;
	return (rtn);
}

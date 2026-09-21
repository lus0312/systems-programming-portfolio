#include "minishell.h"

static int	ft_is_right_quote(char *line)
{
	int		i;
	char	c;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' || line [i] == '"')
		{
			c = line[i++];
			while (line[i] != '\0' && line[i] != c)
				i++;
			if (line[i] != c)
				return (ft_err_rtn(\
				"msh: Quotation don't match.\n", 1));
		}
		if (line[i] != '\0')
			i++;
	}
	return (0);
}

static int	ft_check_double_sign(char *line, int *i)
{
	int		s;
	int		bit;
	char	c;

	s = *i;
	bit = 0;
	c = line[s];
	while ((!bit && line[*i] == c) || line[*i] == ' ')
	{
		if ((c == '&' && *i - s != 1) || (c == '|' && *i - s > 2))
			return (ft_err_rtn(\
			"msh is handle just '|' or '&&' or '||'\n", 1));
		else if (!bit && *i - s > 2)
			return (ft_err_rtn(\
			"msh is handle just '<' or '>' or '<<' or '>>'\n", 1));
		else if (line[*i] == ' ')
			bit = 1;
		*i += 1;
	}
	if (line[*i] == '\0')
		return (ft_err_rtn("msh: Error!: Unexpected token\n", 1));
	c = line[*i];
	if (c == '&' || c == '|' || c == '<' || c == '>')
		return (ft_err_rtn("msh: Error!: Unexpacted token\n", 1));
	return (0);
}

static int	ft_is_right_sign(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '"')
			ft_pass_quotation(line, &i);
		if (line[i] == '\\' || line[i] == ';')
			return (ft_err_rtn(\
			"msh is not handle '\\' or ';'\n", 1));
		if (((line[i] == '&' || line[i] == '|') || \
		(line[i] == '<' || line[i] == '>')) && \
		ft_check_double_sign(line, &i))
			return (1);
		if (line[i] != '\0')
			i++;
	}
	return (0);
}

int	ft_check_right_cmd(char *line, int *status)
{
	if (ft_is_right_quote(line) || \
	ft_is_right_sign(line))
	{
		*status = 2;
		return (1);
	}
	return (0);
}

#include "push_swap.h"

void	handle_empty_string(char **values, t_list **a)
{
	if (values)
		ft_free_split(values);
	if (a)
		free_stack(a);
	print_error();
}

int	is_empty_string(const char *str)
{
	if (*str == '\0')
		return (1);
	while (*str)
	{
		if (*str != ' ')
			return (0);
		str++;
	}
	return (1);
}

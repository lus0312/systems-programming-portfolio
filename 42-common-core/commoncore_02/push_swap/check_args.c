#include "push_swap.h"

int	range_and_dup_check(t_list *a, long num)
{
	int		i;
	int		size;

	if (num < -2147483648 || num > 2147483647)
		return (0);
	i = 0;
	size = ft_lstsize(a);
	while (i < size)
	{
		if (a->value == num)
			return (0);
		i++;
		a = a->next;
	}
	return (1);
}

int	ft_isdigit(char *argv)
{
	int	i;

	i = 0;
	if (argv[i] == '+' || argv[i] == '-')
	{
		i++;
		if (ft_strlen(argv) == 1)
			return (0);
	}
	while (argv[i])
	{
		if (argv[i] < '0' || argv[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	handle_split_values(char **values, t_list **a)
{
	int		i;
	long	num;

	i = 0;
	while (values[i])
	{
		if (!ft_isdigit(values[i]))
		{
			ft_free_split(values);
			if (a)
				free_stack(a);
			print_error();
		}
		num = ft_atoi(values[i]);
		if (*a != NULL && !range_and_dup_check(*a, num))
		{
			ft_free_split(values);
			if (a)
				free_stack(a);
			print_error();
		}
		ft_lstadd_back(a, ft_lstnew(num));
		i++;
	}
}

int	parse_single_value(char *argv, t_list **a)
{
	long	num;

	if (!ft_isdigit(argv))
	{
		if (a)
			free_stack(a);
		return (0);
	}
	num = ft_atoi(argv);
	if (!range_and_dup_check(*a, num))
	{
		if (a)
			free_stack(a);
		return (0);
	}
	ft_lstadd_back(a, ft_lstnew(num));
	return (1);
}

void	parse_args_to_stack(int argc, char **argv, t_list **a)
{
	int		i;
	char	**values;

	i = 1;
	values = NULL;
	while (i < argc)
	{			
		if (is_empty_string(argv[i]))
			handle_empty_string(values, a);
		if (ft_strchr(argv[i], ' '))
		{
			values = ft_split(argv[i], ' ');
			handle_split_values(values, a);
		}
		else
		{
			if (!parse_single_value(argv[i], a))
			{
				ft_free_split(values);
				print_error();
			}
		}	
		i++;
	}
	ft_free_split(values);
}

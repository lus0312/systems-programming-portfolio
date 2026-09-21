#include "push_swap.h"

void	sort_values_ascending(int *values, int size)
{
	int	i;
	int	j;
	int	temp;

	i = 0;
	while (i < size -1)
	{
		j = i + 1;
		while (j < size)
		{
			if (values[i] > values[j])
			{
				temp = values[i];
				values[i] = values[j];
				values[j] = temp;
			}
			j++;
		}
		i++;
	}
}

void	assign_ranks_from_values(int *values, t_list *stack, int size)
{
	int		i;
	t_list	*temp;

	temp = stack;
	while (temp != NULL)
	{
		i = 0;
		while (i < size)
		{
			if (values[i] == temp->value)
			{
				temp->rank = i;
				break ;
			}
			i++;
		}
		temp = temp->next;
		if (temp == stack)
			break ;
	}
}

void	rank_stack_nodes(t_list **stack)
{
	int		size;
	int		*values;
	t_list	*temp;
	int		i;

	size = ft_lstsize(*stack);
	values = (int *)malloc(sizeof(int) * size);
	if (!values)
		return ;
	temp = *stack;
	i = 0;
	while (i < size)
	{
		values[i++] = temp->value;
		temp = temp->next;
	}
	sort_values_ascending(values, size);
	assign_ranks_from_values(values, *stack, size);
	free(values);
}

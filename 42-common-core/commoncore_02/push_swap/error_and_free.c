#include "push_swap.h"

void	free_stack(t_list **stack)
{
	t_list	*temp;
	t_list	*current;

	if (!stack || !*stack)
		return ;
	current = *stack;
	while (current->next != *stack)
	{
		temp = current;
		current = current->next;
		free(temp);
	}
	free(current);
	*stack = NULL;
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i] != NULL)
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	print_error(void)
{
	write(2, "Error\n", 6);
	exit(EXIT_FAILURE);
}

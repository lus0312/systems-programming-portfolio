#include "push_swap.h"

t_list	*ft_lstnew(long value)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->value = value;
	new->rank = -1;
	new->next = new;
	return (new);
}

void	ft_lstadd_front(t_list **stack, t_list *new)
{
	t_list	*last_node;

	if (*stack == NULL)
	{
		*stack = new;
		new->next = *stack;
	}
	else
	{
		last_node = ft_lstlast(*stack);
		if (!last_node)
			return ;
		last_node->next = new;
		new->next = *stack;
		*stack = new;
	}
}

void	ft_lstadd_back(t_list **stack, t_list *new)
{
	t_list	*last_node;

	if (*stack == NULL)
	{
		*stack = new;
		new->next = *stack;
	}
	else
	{
		last_node = ft_lstlast(*stack);
		if (!last_node)
			return ;
		last_node->next = new;
		new->next = *stack;
	}
}

t_list	*ft_lstlast(t_list *stack)
{
	t_list	*current;
	t_list	*start;

	if (!stack)
		return (NULL);
	current = stack;
	start = stack;
	while (current->next != start)
	{
		current = current->next;
		if (current == start)
			break ;
	}
	return (current);
}

int	ft_lstsize(t_list *stack)
{
	int		len;
	t_list	*temp;

	if (!stack)
		return (0);
	temp = stack->next;
	len = 1;
	while (temp != stack)
	{
		temp = temp->next;
		len++;
	}
	return (len);
}

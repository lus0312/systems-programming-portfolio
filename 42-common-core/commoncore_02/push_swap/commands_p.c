#include "push_swap.h"

void	pa(t_list **stack_a, t_list **stack_b)
{
	t_list	*node_to_move;
	t_list	*last_node;

	if (ft_lstsize(*stack_b) == 0)
		return ;
	node_to_move = *stack_b;
	if ((*stack_b)-> next == *stack_b)
		*stack_b = NULL;
	else
	{
		last_node = ft_lstlast(*stack_b);
		last_node->next = (*stack_b)->next;
		*stack_b = (*stack_b)->next;
	}
	ft_lstadd_front(stack_a, node_to_move);
	ft_putendl_fd("pa", 1);
}

void	pb(t_list **stack_a, t_list **stack_b)
{
	t_list	*node_to_move;
	t_list	*last_node;

	if (ft_lstsize(*stack_a) == 0)
		return ;
	node_to_move = *stack_a;
	if ((*stack_a)->next == *stack_a)
		*stack_a = NULL;
	else
	{
		last_node = ft_lstlast(*stack_a);
		last_node->next = (*stack_a)->next;
		*stack_a = (*stack_a)->next;
	}
	if (*stack_b == NULL)
	{
		*stack_b = node_to_move;
		(*stack_b)->next = *stack_b;
	}
	else
		ft_lstadd_front(stack_b, node_to_move);
	ft_putendl_fd("pb", 1);
}

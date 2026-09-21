#include "push_swap.h"

void	ra(t_list **stack_a)
{
	if (*stack_a && ((*stack_a)->next != *stack_a))
	{
		*stack_a = (*stack_a)->next;
		ft_putendl_fd("ra", 1);
	}
}

void	rb(t_list **stack_b)
{
	if (*stack_b && ((*stack_b)->next != *stack_b))
	{
		*stack_b = (*stack_b)->next;
		ft_putendl_fd("rb", 1);
	}
}

void	rra(t_list **stack_a)
{
	t_list	*last;

	if (*stack_a && ((*stack_a)->next != *stack_a))
	{
		last = ft_lstlast(*stack_a);
		*stack_a = last;
		ft_putendl_fd("rra", 1);
	}
}

void	rrb(t_list **stack_b)
{
	t_list	*last;

	if (*stack_b && ((*stack_b)->next != *stack_b))
	{
		last = ft_lstlast(*stack_b);
		*stack_b = last;
		ft_putendl_fd("rrb", 1);
	}
}

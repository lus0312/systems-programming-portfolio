#include "push_swap.h"

int	swap(t_list *stack)
{
	int	tmp_val;
	int	tmp_rank;

	if (ft_lstsize(stack) < 2)
		return (-1);
	tmp_val = stack->value;
	tmp_rank = stack->rank;
	stack->value = stack->next->value;
	stack->rank = stack->next->rank;
	stack->next->value = tmp_val;
	stack->next->rank = tmp_rank;
	return (0);
}

void	sa(t_list *stack_a)
{
	if (swap(stack_a) == -1)
		return ;
	ft_putendl_fd("sa", 1);
}

void	sb(t_list *stack_b)
{
	if (swap(stack_b) == -1)
		return ;
	ft_putendl_fd("sb", 1);
}

void	ss(t_list *stack_a, t_list *stack_b)
{
	if ((ft_lstsize(stack_a) < 2) || ft_lstsize(stack_b) < 2)
		return ;
	swap(stack_a);
	swap(stack_b);
	ft_putendl_fd("ss", 1);
}

#include "push_swap.h"

int	main(int argc, char **argv)
{
	int		size;
	t_stack	stack;

	stack.a = NULL;
	stack.b = NULL;
	if (argc < 2)
		return (0);
	parse_args_to_stack(argc, argv, &stack.a);
	rank_stack_nodes(&stack.a);
	if (is_sorted(stack.a))
	{
		free_stack(&stack.a);
		return (0);
	}
	size = ft_lstsize(stack.a);
	if (size <= 5)
	{
		simple_sort(&stack.a, &stack.b, size);
		free_stack(&stack.a);
		return (0);
	}
	radix_sort(&stack.a, &stack.b, size);
	free_stack(&stack.a);
	return (0);
}

#include "push_swap.h"

int	is_sorted(t_list *a)
{
	t_list	*current;
	t_list	*start;

	current = a;
	start = a;
	while (current->next != start)
	{
		if (current->rank > current->next->rank)
			return (0);
		current = current->next;
	}
	if (current->rank < start->rank)
		return (0);
	return (1);
}

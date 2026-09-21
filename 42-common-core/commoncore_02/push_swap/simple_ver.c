#include "push_swap.h"

void	sort_three(t_list **a)
{
	int	v_1;
	int	v_2;
	int	v_3;

	v_1 = (*a)->value;
	v_2 = (*a)->next->value;
	v_3 = (*a)->next->next->value;
	if (v_1 < v_2 && v_1 > v_3)
		rra(a);
	else if (v_1 < v_3 && v_2 > v_3)
	{
		sa(*a);
		ra(a);
	}
	else if (v_1 > v_2 && v_1 < v_3)
		sa(*a);
	else if (v_1 > v_2 && v_2 < v_3)
		ra(a);
	else if (v_1 > v_2 && v_1 > v_3)
	{
		sa(*a);
		rra(a);
	}
}

void	sort_four_and_five(t_list **a, t_list **b, int size)
{
	int	cnt;
	int	current_rank;

	cnt = 2;
	if (size == 4)
		cnt = 1;
	current_rank = 0;
	while (cnt-- > 0)
	{
		while ((*a) && (*a)->rank != current_rank)
		{
			if ((*a)->next->rank == current_rank)
				sa(*a);
			else if ((*a)->next->next->rank == current_rank)
				ra(a);
			else
				rra(a);
		}
		pb(a, b);
		current_rank++;
	}
	sort_three(a);
	pa(a, b);
	if (size == 5)
		pa(a, b);
}

void	simple_sort(t_list **a, t_list **b, int size)
{
	if (size == 2)
		sa(*a);
	else if (size == 3)
		sort_three(a);
	else
		sort_four_and_five(a, b, size);
}

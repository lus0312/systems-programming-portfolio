#include "push_swap.h"

int	get_max_bit_len(t_list *a, int size)
{
	int	i;
	int	max_rank;
	int	bit_len;

	i = 0;
	max_rank = 0;
	bit_len = 0;
	while (i < size)
	{
		if (max_rank < a->rank)
			max_rank = a->rank;
		i++;
		a = a->next;
	}
	while ((max_rank >> bit_len) != 0)
		bit_len++;
	return (bit_len);
}

void	radix_sort(t_list **a, t_list **b, int size)
{
	int	cnt;
	int	i;
	int	j;

	i = 0;
	cnt = get_max_bit_len(*a, size);
	while (i < cnt)
	{
		j = 0;
		while (j++ < size)
		{
			if ((((*a)->rank >> i) & 1) == 0)
				pb(a, b);
			else
				ra(a);
		}
		while ((*b) != NULL)
			pa(a, b);
		i++;
	}
}

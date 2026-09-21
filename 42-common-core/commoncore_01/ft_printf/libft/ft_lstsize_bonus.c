#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	len;

	if (lst == NULL)
		return (0);
	len = 0;
	while (lst != NULL)
	{
		len++;
		lst = lst->next;
	}
	return (len);
}

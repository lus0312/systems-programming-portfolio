#include "libft.h"

int	ft_memcmp(const void *buf1, const void *buf2, size_t c)
{
	size_t			i;
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	ptr1 = (unsigned char *)buf1;
	ptr2 = (unsigned char *)buf2;
	i = 0;
	while (i < c)
	{
		if (ptr1[i] == ptr2[i])
			i++;
		else
			return (ptr1[i] - ptr2[i]);
	}
	return (0);
}

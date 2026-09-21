#include "libft.h"

void	ft_bzero(void *str, size_t n)
{
	unsigned char	*s;

	s = (unsigned char *)str;
	while (n-- > 0)
		*s++ = 0;
}

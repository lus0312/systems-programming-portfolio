#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;

	str = s;
	while (n-- > 0)
		*(unsigned char *)s++ = c;
	return (str);
}

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	if (s == NULL)
		return (NULL);
	while (n-- > 0)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((void *)s);
		s++;
	}
	return (NULL);
}

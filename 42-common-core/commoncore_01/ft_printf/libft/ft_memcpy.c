#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t count)
{
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	if (dest == src || count == 0)
		return (dest);
	while (count-- > 0)
		*d++ = *s++;
	return (dest);
}

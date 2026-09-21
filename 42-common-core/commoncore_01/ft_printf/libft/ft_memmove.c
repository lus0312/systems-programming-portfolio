#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t count)
{
	void	*result;

	result = dest;
	if (dest == src || count == 0)
		return (result);
	else if (dest < src)
	{
		while (count-- > 0)
			*(unsigned char *)dest++ = *(unsigned char *)src++;
	}
	else
	{
		while (count-- > 0)
			*(unsigned char *)(dest + count) = *(unsigned char *)(src + count);
	}
	return (result);
}

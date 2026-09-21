#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	len = ft_strlen(src);
	if (size == 0)
		return (len);
	while (size > 1 && src[i])
	{
		dst[i] = src[i];
		i++;
		size--;
	}
	dst[i] = '\0';
	return (len);
}

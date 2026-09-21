#include "libft.h"

int	ft_strlcpy(char *dst, const char *src, size_t size)
{
	int				i;
	unsigned int	len;

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

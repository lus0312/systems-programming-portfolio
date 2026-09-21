#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;
	size_t	i;

	i = 0;
	d_len = ft_strlen(dst);
	s_len = ft_strlen(src);
	if (size < d_len)
		return (size + s_len);
	while (*src && (d_len + i + 1 < size))
	{
		*(dst + d_len + i) = *src++;
		i++;
	}
	*(dst + d_len + i) = '\0';
	return (d_len + s_len);
}

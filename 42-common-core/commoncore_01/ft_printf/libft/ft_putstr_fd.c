#include "libft.h"

int	ft_putstr_fd(char *s, int fd)
{
	int	len;

	len = 0;
	if (!s)
		return (ft_putstr_fd("(null)", fd));
	while (*s)
	{
		len += ft_putchar_fd(*s, fd);
		s++;
	}
	return (len);
}

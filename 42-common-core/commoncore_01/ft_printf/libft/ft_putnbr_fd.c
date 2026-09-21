#include "libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	int		len;
	char	nbr;

	len = 0;
	if (n == -2147483648)
		return (write(fd, "-2147483648", 11));
	else if (n < 0)
	{
		len += write(fd, "-", 1);
		n *= -1;
	}	
	if (n >= 10)
		len += ft_putnbr_fd(n / 10, 1);
	nbr = n % 10 + '0';
	len += write(fd, &nbr, 1);
	return (len);
}

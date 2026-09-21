#include "ft_printf.h"

int	ft_putnbr_unsigned(unsigned int nb)
{
	int		len;
	char	nbr;

	len = 0;
	if (nb >= 10)
		len = ft_putnbr_unsigned(nb / 10);
	nbr = nb % 10 + '0';
	len += write(1, &nbr, 1);
	return (len);
}

int	ft_putnbr_hexa(unsigned int nb, char type)
{
	char	*hexa_base;
	int		len;

	len = 0;
	if (type == 'x')
		hexa_base = "0123456789abcdef";
	else if (type == 'X')
		hexa_base = "0123456789ABCDEF";
	else
	{
		hexa_base = NULL;
		return (-1);
	}
	if (nb >= 16)
		len = ft_putnbr_hexa(nb / 16, type);
	len += ft_putchar_fd(hexa_base[nb % 16], 1);
	return (len);
}

int	ft_putptr_hexa(unsigned long long addr)
{
	char	*hexa_base;
	int		len;

	len = 0;
	hexa_base = "0123456789abcdef";
	if (addr >= 16)
		len = ft_putptr_hexa(addr / 16);
	len += ft_putchar_fd(hexa_base[addr % 16], 1);
	return (len);
}

int	ft_putptr(void *ptr)
{
	int					len;
	unsigned long long	addr;

	len = 0;
	addr = (unsigned long long)ptr;
	if (!ptr)
		return (write(1, "(nil)", 5));
	len += ft_putstr_fd("0x", 1);
	len += ft_putptr_hexa(addr);
	return (len);
}

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include "./libft/libft.h"

int	ft_putnbr_unsigned(unsigned int nb);

int	ft_putnbr_hexa(unsigned int nb, char type);

int	ft_putptr(void *ptr);

int	ft_putptr_hexa(unsigned long long addr);

int	ft_conversion(const char type, va_list ap);

int	ft_printf(const char *str, ...);

#endif

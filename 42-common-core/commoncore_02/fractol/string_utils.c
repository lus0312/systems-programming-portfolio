#include "fractol.h"

void	ft_putendl_fd(char *s, int fd)
{
	while (*s)
	{
		if (write(fd, s++, 1) == -1)
			return ;
	}
	if (write(fd, "\n", 1) == -1)
		return ;
}

int	ft_strncmp(const char *s1, const char *s2, size_t num)
{
	size_t	i;

	i = 0;
	while (i < num && (s1[i] || s2[i]))
	{
		if (s1[i] == s2[i])
			i++;
		else if (s1[i] - s2[i] > 0)
			return (1);
		else if (s1[i] - s2[i] < 0)
			return (-1);
	}
	return (0);
}

double	atodbl(char *s)
{
	long	integer_part;
	double	fractional_part;
	double	pow;
	int		sign;

	integer_part = 0;
	fractional_part = 0;
	sign = 1;
	pow = 1;
	while ((*s >= 9 && *s <= 13) || *s == 32)
		s++;
	if (*s == '+' || *s == '-')
		sign = 1 - 2 * (*s++ == '-');
	while (*s != '.' && *s)
		integer_part = (integer_part * 10) + (*s++ - '0');
	if ((*s == '.'))
		s++;
	while (*s)
	{
		pow /= 10;
		fractional_part += pow * (*s++ - '0');
	}
	return ((integer_part + fractional_part) * sign);
}

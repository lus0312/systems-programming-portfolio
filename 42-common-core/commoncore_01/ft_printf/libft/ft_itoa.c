#include "libft.h"

static unsigned int	count_digit(unsigned int dupn)
{
	unsigned int	count;

	count = 0;
	if (dupn == 0)
		return (1);
	while (dupn > 0)
	{
		dupn = dupn / 10;
		count++;
	}
	return (count);
}

static unsigned int	duplicate_n(int n)
{
	unsigned int	dupn;

	dupn = 0;
	if (n < 0)
		dupn = -1 * n;
	else
		dupn = n;
	return (dupn);
}

char	*ft_itoa(int n)
{
	int				i;
	char			*str;
	unsigned int	count;
	unsigned int	dupn;

	count = 0;
	i = 0;
	dupn = duplicate_n(n);
	count = count_digit(dupn);
	if (n < 0)
		i = 1;
	str = (char *)ft_calloc(sizeof(char), (count + i + 1));
	if (!str)
		return (0);
	while (dupn > 0)
	{
		str[count - 1 + i] = dupn % 10 + '0';
		dupn = dupn / 10;
		count--;
	}
	if (n == 0)
		str[0] = '0';
	if (n < 0)
		str[0] = '-';
	return (str);
}

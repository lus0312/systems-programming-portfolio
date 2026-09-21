#include "push_swap.h"

long	ft_atoi(const char *str)
{
	long	result;
	long	cnt;

	cnt = 1;
	result = 0;
	while (*str == 32 || (*str >= 8 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			cnt *= -1;
		str++;
	}
	if (*str >= '0' && *str <= '9')
	{
		while (*str >= '0' && *str <= '9')
		{
			result = result * 10 + (*str - '0');
			str++;
		}
		return (cnt * result);
	}
	else
		return (0);
}

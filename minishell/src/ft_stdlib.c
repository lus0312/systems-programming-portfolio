#include "minishell.h"

static int	ft_numlen(int n)
{
	int	cnt;

	cnt = 0;
	if (n <= 0)
		cnt++;
	while (n != 0)
	{
		n /= 10;
		cnt++;
	}
	return (cnt);
}

char	*ft_itoa(int n)
{
	int			i;
	char		*num;
	long long	ln;

	i = ft_numlen(n);
	ln = n;
	num = (char *)malloc(sizeof(char) * (i + 1));
	if (!num)
		return (NULL);
	if (n < 0)
	{
		num[0] = '-';
		ln = -ln;
	}
	if (n == 0)
		num[0] = '0';
	num[i] = '\0';
	while (ln)
	{
		num[--i] = ln % 10 + '0';
		ln /= 10;
	}
	return (num);
}

int	ft_atoi(const char *nptr, int *err)
{
	int				npbit;
	long long		num;

	num = 0;
	npbit = 1;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			npbit = -1;
		nptr++;
	}
	while (*nptr != '\0')
	{
		if (*nptr < '0' || *nptr > '9')
			*err = 1;
		num = num * 10 + (*nptr - '0');
		if (num * npbit > INT_MAX || num * npbit < INT_MIN)
			*err = 1;
		if (*err)
			return (0);
		nptr++;
	}
	return (num * npbit);
}

int	ft_realloc(char **temp, int *size)
{
	int		i;
	char	*rtn;

	*size += 100;
	rtn = malloc(*size + 2);
	if (!rtn)
		return (free(*temp), 0);
	i = 0;
	if (*temp != NULL)
	{
		while ((*temp)[i] != '\0')
		{
			rtn[i] = (*temp)[i];
			i++;
		}
	}
	while (i < *size + 2)
		rtn[i++] = '\0';
	free(*temp);
	*temp = rtn;
	return (1);
}

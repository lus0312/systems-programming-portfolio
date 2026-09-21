#include "push_swap.h"

static int	countrows(char const *s, char c)
{
	int		count;

	count = 0;
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

void	memory_free(char **res, unsigned int i)
{
	unsigned int	num;

	num = 0;
	while (num < i)
	{
		free(res[num]);
		num++;
	}
	free(res);
}

char	**get_word(char const *s, char c, char **res)
{
	unsigned int	i;
	unsigned int	start;
	size_t			len;

	i = 0;
	start = 0;
	while (s[start])
	{
		if (s[start] == c)
			start++;
		else
		{
			len = 0;
			while (s[start + len] && s[start + len] != c)
				len++;
			res[i++] = ft_substr(s, start, len);
			if (!res[i - 1])
			{
				memory_free(res, i -1);
				return (NULL);
			}
			start += len;
		}
	}
	return (res);
}

char	**ft_split(char const *s, char c)
{
	int		count;
	char	**res;

	if (!s)
		return (NULL);
	count = countrows(s, c);
	res = (char **)malloc(sizeof(char *) * (count + 1));
	if (!res)
		return (NULL);
	res[count] = NULL;
	res = get_word(s, c, res);
	if (!res)
		return (NULL);
	return (res);
}

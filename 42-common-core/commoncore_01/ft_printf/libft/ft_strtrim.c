#include "libft.h"

static int	countstr(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	size_t	count;

	i = 0;
	j = 0;
	count = 0;
	while (set[i])
	{
		while (s1[j] == set[i])
		{
			i = 0;
			count++;
			j++;
		}
		i++;
	}
	return (count);
}

static int	b_countstr(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;
	size_t	count;

	i = 0;
	len = 0;
	count = 0;
	len = ft_strlen(s1);
	while (set[i])
	{
		while (s1[len -1] == set[i])
		{
			i = 0;
			count++;
			len--;
		}
		i++;
	}
	return (count);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	size_t	start;
	size_t	end;
	char	*arr;

	len = ft_strlen(s1);
	start = countstr(s1, set);
	end = b_countstr(s1, set);
	if (start + end > len)
	{
		arr = ft_strdup("");
		return (arr);
	}
	arr = (char *)malloc(sizeof(char) * (len - end - start + 1));
	if (!arr)
		return (0);
	ft_strlcpy(arr, s1 + start, len - end - start + 1);
	return (arr);
}

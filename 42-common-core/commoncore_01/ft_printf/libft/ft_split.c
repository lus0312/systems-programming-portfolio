#include "libft.h"

static int	countrow(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s != c && *s)
		{
			count++;
			while (*s != c && *s)
				s++;
		}
	}
	return (count);
}

void	ft_free(char **str, int i)
{
	while (i > 0)
		free (str[--i]);
	free(str);
}

static int	count_length(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s != c && *s)
	{
		s++;
		count++;
	}
	return (count);
}

char	**insultstr(char **str, char const *s, char c)
{
	int	count;
	int	i;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		count = count_length(s, c);
		if (count > 0)
		{
			str[i] = (char *)malloc(sizeof(char) * (count + 1));
			if (!str[i])
			{
				ft_free(str, i);
				return (NULL);
			}
			ft_strlcpy(str[i++], s, count + 1);
			s += count;
		}
	}
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		count;

	if (!s)
		return (NULL);
	count = countrow(s, c);
	str = (char **)malloc(sizeof(char *) * (count + 1));
	if (!str)
		return (NULL);
	str[count] = NULL;
	str = insultstr(str, s, c);
	return (str);
}

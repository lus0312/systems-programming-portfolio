#include "pipex.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str && str[len] != '\0')
		len++;
	return (len);
}

int	ft_printf(const char *str)
{
	return (write(1, str, ft_strlen(str)));
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

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*arr;
	size_t	len_1;
	size_t	len_2;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	len_1 = ft_strlen(s1);
	len_2 = ft_strlen(s2);
	arr = (char *)malloc(sizeof(char) * (len_1 + len_2 + 1));
	if (!arr)
		return (0);
	while (*s1)
	{
		arr[i] = *s1++;
		i++;
	}
	while (*s2)
	{
		arr[i] = *s2++;
		i++;
	}
	arr[i] = '\0';
	return (arr);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*arr;
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	if (start >= (unsigned int)ft_strlen(s))
		len = 0;
	else if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (!arr)
		return (0);
	while (i < len)
	{
		arr[i] = s[start + i];
		i++;
	}
	arr[i] = '\0';
	return (arr);
}

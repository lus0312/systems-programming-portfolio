#include "get_next_line_bonus.h"

char	*gnl_strjoin(char *s1, char *s2)
{
	char	*arr;
	size_t	len_1;
	size_t	len_2;

	if (!s1)
	{
		s1 = (char *)malloc(1);
		if (!s1)
			return (NULL);
		s1[0] = '\0';
	}
	len_1 = ft_strlen(s1);
	len_2 = ft_strlen(s2);
	arr = (char *)malloc(len_1 + len_2 + 1);
	if (!arr)
	{
		free (s1);
		return (NULL);
	}
	ft_strlcpy(arr, s1, len_1 + 1);
	ft_strlcpy(arr + len_1, s2, len_2 + 1);
	free (s1);
	return (arr);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	len = ft_strlen(src);
	if (size == 0)
		return (len);
	while (size > 1 && src[i])
	{
		dst[i] = src[i];
		i++;
		size--;
	}
	dst[i] = '\0';
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*arr;
	size_t	i;

	if (!s)
		return (0);
	if (start >= (unsigned int)ft_strlen(s))
		len = 0;
	else if (len > ft_strlen(s))
		len = ft_strlen(s);
	arr = (char *)malloc(len + 1);
	if (!arr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		arr[i] = s[start + i];
		i++;
	}
	arr[i] = '\0';
	return (arr);
}

char	*ft_strchr(const char *str, int c)
{
	while (*str)
	{
		if (*str == (char)c)
			return ((char *)str);
		str++;
	}
	if (c == '\0')
		return ((char *)str);
	return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str && str[len] != '\0')
		len++;
	return (len);
}

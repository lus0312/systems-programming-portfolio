#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*arr;
	size_t	len_1;
	size_t	len_2;
	size_t	i;

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

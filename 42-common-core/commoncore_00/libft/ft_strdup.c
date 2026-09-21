#include "libft.h"

char	*ft_strdup(const char *string)
{
	int		i;
	int		len;
	char	*arr;

	i = 0;
	len = ft_strlen(string);
	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (!arr)
		return (0);
	else
	{
		while (i < len)
		{
			arr[i] = string[i];
			i++;
		}
		arr[i] = '\0';
	}
	return (arr);
}

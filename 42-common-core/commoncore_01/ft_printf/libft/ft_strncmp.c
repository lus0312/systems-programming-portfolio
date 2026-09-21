#include "libft.h"

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

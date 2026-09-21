#include "minishell.h"

int	ft_strlen(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	int	i;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

char	*ft_strdup(const char *s)
{
	char	*addr;
	char	*t;
	int		i;

	i = 0;
	t = (char *)s;
	addr = (char *)malloc(sizeof(char) * (ft_strlen(t) + 1));
	if (addr == 0)
		return (0);
	while (t[i] != '\0')
	{
		addr[i] = t[i];
		i++;
	}
	addr[i] = '\0';
	return (addr);
}

char	*ft_strndup(const char *s, int len)
{
	char	*addr;
	char	*t;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	if (!len)
		return (ft_strdup(""));
	t = (char *)s;
	addr = (char *)malloc(sizeof(char) * (len + 1));
	if (addr == 0)
		return (0);
	while (i < len)
	{
		addr[i] = t[i];
		i++;
	}
	addr[i] = '\0';
	return (addr);
}

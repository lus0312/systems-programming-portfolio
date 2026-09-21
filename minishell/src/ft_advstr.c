#include "minishell.h"

static int	ft_get_height(const char *str, char c)
{
	int	i;
	int	cnt;

	i = 0;
	cnt = 0;
	while (str[i] != '\0')
	{
		if (str[i] != c)
		{
			cnt++;
			while (str[i] && str[i] != c)
				i++;
		}
		if (str[i] != '\0')
			i++;
	}
	return (cnt + 1);
}

static char	*ft_split_helper(const char *str, int *i, char c)
{
	int		idx;
	int		s;
	int		len;
	char	*temp;

	s = *i;
	while (str[*i] != '\0' && str[*i] != c)
		(*i)++;
	len = *i - s;
	temp = (char *)malloc(len + 1);
	if (!temp)
		return (NULL);
	idx = 0;
	while (idx < len)
		temp[idx++] = str[s++];
	temp[idx] = '\0';
	return (temp);
}

char	**ft_split(const char *str, char c)
{
	int		idx;
	int		i;
	char	**arr;

	i = 0;
	idx = 0;
	arr = (char **)malloc(sizeof(char *) * (ft_get_height(str, c)));
	if (!arr)
		return (NULL);
	while (str[i] != '\0')
	{
		if (str[i] != c)
		{
			arr[idx] = ft_split_helper(str, &i, c);
			if (!arr[idx])
				return (ft_free_str(arr), NULL);
			idx++;
		}
		if (str[i] != '\0')
			i++;
	}
	arr[idx] = NULL;
	return (arr);
}

char	*ft_straddjoin(const char *s1, const char *s2, char add)
{
	int		i;
	int		j;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	if (!str)
		return (NULL);
	i = -1;
	while (s1[++i] != '\0')
		str[i] = s1[i];
	if (add)
		str[i++] = add;
	j = 0;
	while (s2[j] != '\0')
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}

char	*ft_strreplace(char *str, int start, int end, char *rep)
{
	char	*front;
	char	*other;
	char	*temp;
	char	*rtn;

	front = ft_strndup(str, start);
	if (!front)
		return (NULL);
	other = ft_strdup(&str[end]);
	if (!other)
		return (free(front), NULL);
	temp = ft_straddjoin(front, rep, 0);
	if (!temp)
		return (free(front), free(other), NULL);
	rtn = ft_straddjoin(temp, other, 0);
	if (!rtn)
		return (free(temp), NULL);
	free(front);
	free(other);
	free(temp);
	return (rtn);
}

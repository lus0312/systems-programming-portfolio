#include "pipex.h"

static int	skip_word(char const *s, int i)
{
	char	quote;

	quote = 0;
	while (s[i] && (quote || s[i] != ' '))
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		i++;
	}
	return (i);
}

static char	*copy_word(char const *s, int start, int end)
{
	char	*word;
	char	quote;
	int		j;

	word = (char *)malloc(end - start + 1);
	if (!word)
		return (NULL);
	quote = 0;
	j = 0;
	while (start < end)
	{
		if (!quote && (s[start] == '\'' || s[start] == '"'))
			quote = s[start];
		else if (quote && s[start] == quote)
			quote = 0;
		else
			word[j++] = s[start];
		start++;
	}
	word[j] = '\0';
	return (word);
}

static int	count_words(char const *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == ' ')
			i++;
		else
		{
			count++;
			i = skip_word(s, i);
		}
	}
	return (count);
}

char	**split_cmd(char const *s)
{
	char	**res;
	int		i;
	int		n;
	int		end;

	res = (char **)malloc(sizeof(char *) * (count_words(s) + 1));
	if (!res)
		return (NULL);
	i = 0;
	n = 0;
	while (s[i])
	{
		if (s[i] == ' ')
			i++;
		else
		{
			end = skip_word(s, i);
			res[n] = copy_word(s, i, end);
			if (!res[n++])
				return (memory_free(res, n - 1), NULL);
			i = end;
		}
	}
	res[n] = NULL;
	return (res);
}

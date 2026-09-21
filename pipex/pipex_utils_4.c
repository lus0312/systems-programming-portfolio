#include "pipex.h"

char	*ft_strchr_p(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_strdup_p(const char *s)
{
	char	*dup;
	size_t	i;

	dup = (char *)malloc(ft_strlen(s) + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	cmd_error(t_pipex *pipex, char **cmd, char *msg, int code)
{
	write(STDERR_FILENO, "pipex: ", 7);
	if (cmd && cmd[0])
		write(STDERR_FILENO, cmd[0], ft_strlen(cmd[0]));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	clean_up(pipex);
	exit(code);
}

void	exec_fail(t_pipex *pipex, char **cmd)
{
	perror(cmd[0]);
	clean_up(pipex);
	exit(126);
}

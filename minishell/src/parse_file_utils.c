#include "minishell.h"

int	ft_parse_file_in(t_data *data, t_list **div, \
	char *redirect, int role)
{
	int	fd;

	if (role == REDIR_READ)
	{
		fd = open(redirect, O_RDONLY);
		if (fd < 0)
		{
			write(STDERR_FILENO, "msh: ", 5);
			perror(redirect);
			return (1);
		}
	}
	else
	{
		fd = ft_redirect_heredoc(data, div, redirect);
		if (fd < 0)
			return (1);
	}
	if ((data->now)->fd)
		close((data->now)->fd);
	(data->now)->redir = role;
	(data->now)->fd = fd;
	return (0);
}

int	ft_parse_file_out(t_list **cmd, char *redirect, int role)
{
	int	fd;

	if (role == REDIR_TRUNC)
	{
		fd = open(redirect, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd < 0)
			return (ft_err_rtn(\
			"msh: Error!: fail to open file", 1));
	}
	else
	{
		fd = open(redirect, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd < 0)
			return (ft_err_rtn(\
			"msh: Error!: fail to open file", 1));
	}
	if ((*cmd)->fd)
		close((*cmd)->fd);
	(*cmd)->redir = role;
	(*cmd)->fd = fd;
	return (0);
}

int	ft_is_delimiter(char *line, char *eof)
{
	int	len;

	len = ft_strlen(line);
	if (len && line[len - 1] == '\n')
		len--;
	return (len == ft_strlen(eof) && !ft_strncmp(line, eof, len));
}

void	ft_heredoc_write(t_data *data, char *line)
{
	char	*quoted;
	char	*expand;
	char	*temp;

	if (data->quoted)
		return ((void)write(STDOUT_FILENO, line, ft_strlen(line)));
	line[ft_strlen(line) - 1] = '\0';
	temp = ft_straddjoin("\"", line, 0);
	quoted = NULL;
	if (temp)
		quoted = ft_straddjoin(temp, "\"", 0);
	free(temp);
	expand = NULL;
	if (quoted)
		expand = ft_parse_quote(quoted, *(data->env), data->status);
	free(quoted);
	if (!expand)
		return ;
	write(STDOUT_FILENO, expand, ft_strlen(expand));
	write(STDOUT_FILENO, "\n", 1);
	free(expand);
}

#include "get_next_line_bonus.h"

char	*update_backup(int fd, char **backup)
{
	char	*temp;
	size_t	index;
	size_t	len;

	index = 0;
	len = ft_strlen(backup[fd]);
	while (backup[fd][index] && backup[fd][index] != '\n')
		index++;
	if (backup[fd][index] == '\n')
		temp = ft_substr(backup[fd], index + 1, len - index - 1);
	else
		temp = NULL;
	if (backup[fd])
	{
		free(backup[fd]);
		backup[fd] = NULL;
	}
	return (temp);
}

char	*extract_line(int fd, char **backup)
{
	char	*line;
	size_t	index;

	index = 0;
	while (backup[fd][index] && backup[fd][index] != '\n')
		index++;
	if (backup[fd][index] == '\n')
		line = ft_substr(backup[fd], 0, index + 1);
	else
		line = ft_substr(backup[fd], 0, index);
	if (!line)
		return (NULL);
	return (line);
}

char	*read_line(int fd, char *buffer, char **backup)
{
	ssize_t	byte_read;

	byte_read = 0;
	if (backup[fd] && ft_strchr(backup[fd], '\n'))
		return (backup[fd]);
	while (fd >= 0)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read < 0)
		{
			free(backup[fd]);
			return (NULL);
		}
		else if (byte_read == 0)
			break ;
		buffer[byte_read] = '\0';
		backup[fd] = gnl_strjoin(backup[fd], buffer);
		if (!backup[fd])
			return (NULL);
		if (ft_strchr(buffer, '\n') != NULL)
			break ;
	}
	if (byte_read == 0 && !backup[fd])
		return (NULL);
	return (backup[fd]);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	static char	*backup[OPEN_MAX];

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	backup[fd] = read_line(fd, buffer, backup);
	free (buffer);
	if (!backup[fd])
		return (NULL);
	if (backup[fd][0] == '\0')
	{
		free (backup[fd]);
		backup[fd] = NULL;
		return (NULL);
	}
	line = extract_line(fd, backup);
	backup[fd] = update_backup(fd, backup);
	return (line);
}

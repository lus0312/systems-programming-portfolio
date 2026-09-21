#include "get_next_line.h"

//read and copy to backup
char	*read_line(int fd, char *buffer, char *backup)
{
	ssize_t	byte_read;

	byte_read = 0;
	if (backup && ft_strchr(backup, '\n'))
		return (backup);
	while (fd >= 0)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read < 0)
		{
			free(backup);
			return (NULL);
		}
		else if (byte_read == 0)
			break ;
		buffer[byte_read] = '\0';
		backup = gnl_strjoin(backup, buffer);
		if (!backup)
			return (NULL);
		if (ft_strchr(buffer, '\n') != NULL)
			break ;
	}
	return (backup);
}

//extract line from backup
char	*extract_line(char	*backup)
{
	char	*line;
	size_t	index;

	index = 0;
	while (backup[index] && backup[index] != '\n')
		index++;
	if (backup[index] == '\n')
		line = ft_substr(backup, 0, index + 1);
	else
		line = ft_substr(backup, 0, index);
	if (!line)
		return (NULL);
	return (line);
}

//update backup
char	*update_backup(char *backup)
{
	char	*temp;
	size_t	index;

	index = 0;
	while (backup[index] && backup[index] != '\n')
		index++;
	if (backup[index] == '\n')
		temp = ft_substr(backup, index + 1, ft_strlen(backup) - index - 1);
	else
		temp = NULL;
	if (backup)
	{
		free(backup);
		backup = NULL;
	}
	return (temp);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*backup;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	backup = read_line(fd, buffer, backup);
	free (buffer);
	if (!backup)
		return (NULL);
	if (backup[0] == '\0')
	{
		free (backup);
		backup = NULL;
		return (NULL);
	}
	line = extract_line(backup);
	backup = update_backup(backup);
	return (line);
}

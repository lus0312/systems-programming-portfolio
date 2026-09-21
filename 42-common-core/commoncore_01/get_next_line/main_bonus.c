#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "get_next_line_bonus.h"

int	main(void)
{
	int		fd_1;
	int		fd_2;
	int		fd_3;
	int		i;
	char		*line;

	fd_1 = open("tests/test1.txt", O_RDONLY);
	fd_2 = open("tests/test2.txt", O_RDONLY);
	fd_3 = open("tests/test3.txt", O_RDONLY);
	if (fd_1 == -1 || fd_2 == -1 || fd_3 == -1)
	{
		perror("Failed to open a file");
		return (1);
	}
	i = 0;
	while (i < 7)
	{
		line = get_next_line(fd_1);
		printf("test1: %s", line);
		free(line);
		line = get_next_line(fd_2);
		printf("test2: %s", line);
		free(line);		
		line = get_next_line(fd_3);
		printf("test3: %s", line);
		free(line);
		i++;
	}
	close(fd_1);
	close(fd_2);
	close(fd_3);
	return (0);
}


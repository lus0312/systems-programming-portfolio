#include "pipex.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	clean_up(t_pipex *pipex)
{
	if (pipex->infile != -1)
		close(pipex->infile);
	if (pipex->outfile != -1)
		close(pipex->outfile);
	if (pipex->paths)
		free_array(pipex->paths);
	if (pipex->cmd1)
		free_array(pipex->cmd1);
	if (pipex->cmd2)
		free_array(pipex->cmd2);
}

void	handle_error(char *str, t_pipex *pipex)
{
	perror(str);
	if (pipex)
		clean_up(pipex);
	exit(1);
}

void	open_files(t_pipex *pipex, char **argv)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile == -1)
		perror(argv[1]);
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile == -1)
		perror(argv[4]);
}

void	init_t_pipex(t_pipex *pipex)
{
	pipex->infile = -1;
	pipex->outfile = -1;
	pipex->cmd1 = NULL;
	pipex->cmd2 = NULL;
	pipex->paths = NULL;
}

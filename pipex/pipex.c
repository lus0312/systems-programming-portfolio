#include "pipex.h"

static int	no_file(t_pipex *pipex, int num)
{
	if (num == 1)
		return (pipex->infile == -1);
	return (pipex->outfile == -1);
}

pid_t	pipe_process(t_pipex *pipex, int num, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		handle_error("fork failed", pipex);
	if (pid == 0)
	{
		if (no_file(pipex, num))
		{
			clean_up(pipex);
			exit(1);
		}
		find_cmd_path(pipex, num, argv);
		execute_cmd(pipex, num, envp);
	}
	return (pid);
}

void	parse_envp(char **envp, t_pipex *pipex)
{
	char	*envp_path;
	int		i;

	envp_path = "";
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			envp_path = envp[i] + 5;
			break ;
		}
		i++;
	}
	pipex->paths = ft_split(envp_path, ':');
	if (!pipex->paths)
		handle_error("PATH parsing failed", pipex);
}

static int	wait_children(t_pipex *pipex)
{
	int	status1;
	int	status2;

	status2 = 0;
	if (waitpid(pipex->pid1, &status1, 0) == -1)
		handle_error("waitpid for pid1 failed", pipex);
	if (waitpid(pipex->pid2, &status2, 0) == -1)
		handle_error("waitpid for pid2 failed", pipex);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	if (WIFSIGNALED(status2))
		return (128 + WTERMSIG(status2));
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		status;

	if (argc != 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
		exit(1);
	}
	init_t_pipex(&pipex);
	open_files(&pipex, argv);
	parse_envp(envp, &pipex);
	if (pipe(pipex.pipefd) == -1)
		handle_error("pipe creation failed", &pipex);
	pipex.pid1 = pipe_process(&pipex, 1, argv, envp);
	pipex.pid2 = pipe_process(&pipex, 2, argv, envp);
	close(pipex.pipefd[0]);
	close(pipex.pipefd[1]);
	status = wait_children(&pipex);
	clean_up(&pipex);
	return (status);
}

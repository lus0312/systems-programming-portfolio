#include "pipex.h"

void	execute_cmd(t_pipex *pipex, int num, char **envp)
{
	if (num == 1)
	{
		close(pipex->pipefd[0]);
		if (dup2(pipex->infile, STDIN_FILENO) == -1)
			handle_error("pid1 dup2 infile failed", pipex);
		if (dup2(pipex->pipefd[1], STDOUT_FILENO) == -1)
			handle_error("pid1 dup2 output failed", pipex);
		close(pipex->pipefd[1]);
		close(pipex->infile);
		close(pipex->outfile);
		execve(pipex->cmd1[0], pipex->cmd1, envp);
		exec_fail(pipex, pipex->cmd1);
	}
	close(pipex->pipefd[1]);
	if (dup2(pipex->pipefd[0], STDIN_FILENO) == -1)
		handle_error("pid2 dup2 input failed", pipex);
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		handle_error("pid2 dup2 outfile failed", pipex);
	close(pipex->pipefd[0]);
	close(pipex->infile);
	close(pipex->outfile);
	execve(pipex->cmd2[0], pipex->cmd2, envp);
	exec_fail(pipex, pipex->cmd2);
}

char	**parse_cmd(char *cmd)
{
	char	**cmd_path;

	if (!cmd)
		handle_error("cmd is NULL", NULL);
	cmd_path = split_cmd(cmd);
	if (!cmd_path)
		handle_error("cmd parsing failed", NULL);
	return (cmd_path);
}

static char	*search_path(t_pipex *pipex, char *name)
{
	int		i;
	char	*temp;
	char	*cmd_path;

	i = 0;
	while (pipex->paths[i])
	{
		temp = ft_strjoin(pipex->paths[i], "/");
		cmd_path = ft_strjoin(temp, name);
		free(temp);
		if (cmd_path && access(cmd_path, X_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*access_check(t_pipex *pipex, char **cmd)
{
	char	*cmd_path;

	if (!cmd[0] || !cmd[0][0])
		cmd_error(pipex, cmd, "command not found", 127);
	if (ft_strchr_p(cmd[0], '/'))
	{
		if (access(cmd[0], F_OK) != 0)
			cmd_error(pipex, cmd, "No such file or directory", 127);
		if (access(cmd[0], X_OK) != 0)
			cmd_error(pipex, cmd, "Permission denied", 126);
		cmd_path = ft_strdup_p(cmd[0]);
	}
	else
		cmd_path = search_path(pipex, cmd[0]);
	if (!cmd_path)
		cmd_error(pipex, cmd, "command not found", 127);
	return (cmd_path);
}

void	find_cmd_path(t_pipex *pipex, int num, char **argv)
{
	char	**cmd;
	char	*cmd_path;

	if (num == 1)
		cmd = parse_cmd(argv[2]);
	else
		cmd = parse_cmd(argv[3]);
	if (num == 1)
		pipex->cmd1 = cmd;
	else
		pipex->cmd2 = cmd;
	cmd_path = access_check(pipex, cmd);
	free(cmd[0]);
	cmd[0] = cmd_path;
}

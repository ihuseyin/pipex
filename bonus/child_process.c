#include "pipex_bonus.h"

void	execution(char **argv, char **envp, int index)
{
	char	**path;
	char	**exec_args;
	int		i;

	exec_args = ft_split(argv[index + 2], ' ');
	path = find_pathenv(exec_args[0], envp);
	if (!path || !exec_args)
	{
		ft_free(path, 0);
		ft_free(exec_args, 0);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (path[i])
		if (execve(path[i], exec_args, NULL) == -1)
			i++;
	write(2, exec_args[0], ft_strlen(exec_args[0]));
	write(2, ": command not found\n", 20);
	ft_free(path, 0);
	ft_free(exec_args, 0);
	exit(127);
}

static void	redirection_out(char **argv, char **envp, int *pipe_fd, int index)
{
	int	fd;

	if (access(argv[index + 3], F_OK) == 0)
	{
		if (access(argv[index + 3], W_OK) == -1)
			error(argv[index + 3], pipe_fd);
	}
	fd = open(argv[index + 3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error("Open output file failed", pipe_fd);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		error("Dup2 stdout failed", pipe_fd);
	}
	close(fd);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		error("Dup2 stdin failed", pipe_fd);
	close(pipe_fd[0]);
	execution(argv, envp, index);
}

static void	redirection_in(char **argv, char **envp, int *pipe_fd, int index)
{
	int	fd;

	if (access(argv[1], F_OK | R_OK) == -1)
		error(argv[1], pipe_fd);
	else
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			error("Open input file failed", pipe_fd);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			error("Dup2 stdin failed", pipe_fd);
		}
		close(fd);
	}
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		error("Dup2 stdout failed", pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execution(argv, envp, index);
}

static void	commands(char **argv, char **envp, int pipefd[SIZE][2], int index)
{
	if (dup2(pipefd[index - 1][0], STDIN_FILENO) == -1)
	{
		close(pipefd[index - 1][0]);
		error("Dup2 stdin failed", pipefd[index]);
	}
	if (dup2(pipefd[index][1], STDOUT_FILENO) == -1)
	{
		close(pipefd[index - 1][0]);
		error("Dup2 stdout failed", pipefd[index]);
	}
	close(pipefd[index - 1][0]);
	close(pipefd[index][0]);
	close(pipefd[index][1]);
	execution(argv, envp, index);
}

void	child_process(char **argv, char **envp, int pipefd[SIZE][2], int index)
{
	if (index == 0)
		redirection_in(argv, envp, pipefd[index], index);
	else if (index == pipefd[SIZE - 1][0])
	{
		close(pipefd[index][0]);
		close(pipefd[index][1]);
		redirection_out(argv, envp, pipefd[index - 1], index);
	}
	else
		commands(argv, envp, pipefd, index);
}

#include "pipex.h"

static void	exec(char **argv, char **envp, int index)
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
		error("Open output file", pipe_fd);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		error("Dup2 out1", pipe_fd);
	}
	close(fd);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		error("Dup2 out2", pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exec(argv, envp, index);
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
			error("Open input file", pipe_fd);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			error("Dup2 in1", pipe_fd);
		}
		close(fd);
	}
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		error("Dup2 in2", pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exec(argv, envp, index);
}

static int	execution(int argc, char **argv, char **envp, int *pipe_fd)
{
	int	index;
	int	pid;
	int	status;

	index = 0;
	while (argc - 3 > index)
	{
		pid = fork();
		if (pid == -1)
			error("Fork", pipe_fd);
		if (pid == 0)
		{
			if (index == 0)
				redirection_in(argv, envp, pipe_fd, index);
			else if (index == argc - 4)
				redirection_out(argv, envp, pipe_fd, index);
		}
		waitpid(pid, &status, 0);
		close(pipe_fd[1]);
		pipe_fd[1] = -1;
		index++;
	}
	return (WEXITSTATUS(status));
}

int	main(int argc, char *argv[], char *envp[])
{
	int	pipe_fd[2];
	int	status;

	if (argc != 5)
	{
		errno = EINVAL;
		error("Usage: ./pipex file1 cmd1 cmd2 file2\nError", 0);
	}
	if (pipe(pipe_fd) == -1)
		error("Pipe", 0);
	status = execution(argc, argv, envp, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (status);
}

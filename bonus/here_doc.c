#include "pipex_bonus.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

static void	search_limiter(char *limiter, int *pipefd)
{
	char	*line;

	line = NULL;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!ft_strncmp(line, limiter, ft_strlen(limiter)))
			break ;
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	get_next_line(-100);
	free(line);
	close(pipefd[1]);
}

static void	last_cmd(char **argv, char **envp, int *pipefd, int *status)
{
	int	pid;
	int	fd;

	fd = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		error(argv[5], pipefd);
	pid = fork();
	if (pid == -1)
		error("Fork failed", pipefd);
	if (pid == 0)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			error("Dup2 stdout failed", pipefd);
		}
		close(fd);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			error("Dup2 stdin failed", pipefd);
		close(pipefd[0]);
		execution(argv, envp, 2);
	}
	waitpid(pid, status, 0);
	close(pipefd[0]);
}

static void	first_cmd(char **argv, char **envp, int pipefd[2][2], int *status)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0][0]);
		error("Fork failed", pipefd[1]);
	}
	if (pid == 0)
	{
		if (dup2(pipefd[0][0], STDIN_FILENO) == -1)
		{
			close(pipefd[0][0]);
			error("Dup2 stdin failed", pipefd[1]);
		}
		close(pipefd[0][0]);
		if (dup2(pipefd[1][1], STDOUT_FILENO) == -1)
			error("Dup2 stdout failed", pipefd[1]);
		close(pipefd[1][0]);
		close(pipefd[1][1]);
		execution(argv, envp, 1);
	}
	waitpid(pid, status, 0);
	close(pipefd[0][0]);
}

int	here_doc(int argc, char **argv, char **envp)
{
	int	pipefd[2][2];
	int	status;

	if (pipe(pipefd[0]) == -1)
		error("Pipe failed", 0);
	search_limiter(argv[2], pipefd[0]);
	if (pipe(pipefd[1]) == -1)
	{
		close(pipefd[0][0]);
		error("Pipe failed", 0);
	}
	first_cmd(argv, envp, pipefd, &status);
	close(pipefd[1][1]);
	last_cmd(argv, envp, pipefd[1], &status);
	return (WEXITSTATUS(status));
}

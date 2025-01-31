#include "pipex_bonus.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

static int	par_process(int argc, char **argv, char **envp, int pipefd[SIZE][2])
{
	int	status;
	int	index;
	int	pid;

	index = 0;
	while (argc - 3 > index)
	{
		pid = fork();
		if (pid == -1)
		{
			close(pipefd[index - 1][0]);
			error("Fork failed", pipefd[index]);
		}
		if (pid == 0)
			child_process(argv, envp, pipefd, index);
		close(pipefd[index][1]);
		close(pipefd[index - 1][0]);
		waitpid(pid, &status, 0);
		if (argc - 3 > ++index)
			if (pipe(pipefd[index]) == -1)
				error("Pipe failed", pipefd[index - 1]);
	}
	close(pipefd[index - 1][0]);
	return (WEXITSTATUS(status));
}

int	main(int argc, char *argv[], char *envp[])
{
	int	pipefd[SIZE][2];
	int	status;

	if (argc < 5 || argc - 2 > SIZE)
	{
		errno = EINVAL;
		error("Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2\nError", 0);
	}
	if (!ft_strncmp(argv[1], "here_doc", 8))
	{
		if (argc != 6)
		{
			errno = EINVAL;
			error("Usage: ./pipex here_doc LIMITER cmd1 cmd2 file\nError", 0);
		}
		status = here_doc(argv, envp);
	}
	else
	{
		pipefd[SIZE - 1][0] = argc - 4;
		if (pipe(pipefd[0]) == -1)
			error("First pipe failed", 0);
		status = par_process(argc, argv, envp, pipefd);
	}
	return (status);
}

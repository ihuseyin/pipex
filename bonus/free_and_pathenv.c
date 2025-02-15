#include "pipex_bonus.h"
#include <stdio.h>
#include <unistd.h>

void	error(char *message, int pipe_fd[])
{
	perror(message);
	if (pipe_fd != NULL)
	{
		if (pipe_fd[0] > 0)
			close(pipe_fd[0]);
		if (pipe_fd[1] > 0)
			close(pipe_fd[1]);
	}
	exit(EXIT_FAILURE);
}

int	ft_strncmp(const char *s1, const char *s2, size_t len)
{
	if (len == 0)
		return (0);
	while (*s1 && *s2 && len - 1 > 0)
	{
		if (*s1 != *s2)
		{
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		}
		s1++;
		s2++;
		len--;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

char	**find_pathenv(char *command, char *envp[])
{
	char	**full_path;
	char	*slash_cmd;
	int		i;
	int		j;

	if (!command)
		return (NULL);
	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	full_path = ft_split((envp[i] + 5), ':');
	slash_cmd = malloc(ft_strlen(command) + 2);
	if (!slash_cmd)
		return (NULL);
	ft_strncpy(slash_cmd, "/", 1);
	ft_strncpy(slash_cmd + 1, command, ft_strlen(command));
	j = 0;
	while (full_path[j])
	{
		full_path[j] = ft_strjoin(full_path[j], slash_cmd);
		j++;
	}
	free(slash_cmd);
	return (full_path);
}

#include "pipex.h"

size_t	ft_strlen(const char *s)
{
	size_t	length;

	length = 0;
	while (*s++)
		length++;
	return (length);
}

static char	*ft_strjoin(char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, s1, len1);
	ft_strncpy(result + len1, s2, len2);
	free(s1);
	return (result);
}

static int	ft_strncmp(const char *s1, const char *s2, size_t len)
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

void	error(char *message, int pipe_fd[])
{
	if (pipe_fd != NULL)
	{
		if (pipe_fd[0] > 0)
			close(pipe_fd[0]);
		if (pipe_fd[1] > 0)
			close(pipe_fd[1]);
	}
	perror(message);
	exit(EXIT_FAILURE);
}

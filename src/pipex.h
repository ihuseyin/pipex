#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>

size_t	ft_strlen(const char *s);
char	**find_pathenv(char *command, char *envp[]);
void	error(char *message, int pipe_fd[]);

void	ft_free(char **result, size_t index);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	**ft_split(const char *s, char c);
#endif

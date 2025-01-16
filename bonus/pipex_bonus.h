#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>

// maximum accessible pipe size
# ifndef SIZE
#  define SIZE 40
# endif

void	error(char *message, int pipe_fd[]);
int		ft_strncmp(const char *s1, const char *s2, size_t len);
char	**find_pathenv(char *command, char *envp[]);

void	child_process(char **argv, char **envp, int pipefd[SIZE][2], int index);

void	ft_free(char **result, size_t index);
char	**ft_split(const char *s, char c);

void	execution(char **argv, char **envp, int index);
int		here_doc(int argc, char **argv, char **envp);

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char *s1, const char *s2);

char	*get_next_line(int fd);
#endif

#include "pipex.h"

void	ft_free(char **result, size_t index)
{
	size_t	i;

	if (!result)
		return ;
	if (index == 0)
	{
		i = 0;
		while (result[i])
			free(result[i++]);
	}
	else
	{
		while (index > 0)
			free(result[--index]);
	}
	free(result);
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static int	ft_count(const char *str, char c)
{
	int	len;

	if (!str)
		return (-1);
	len = 0;
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str == '\'')
		{
			str++;
			while (*str != '\'')
				str++;
		}
		if (*str)
			len++;
		while (*str != c && *str)
			str++;
	}
	return (len);
}

static int	ft_allocate(char **res, const char *srt, const char *end, size_t *i)
{
	if (*srt == '\'')
	{
		srt++;
		end--;
	}
	res[*i] = malloc(end - srt + 1);
	if (res[*i] == NULL)
		return (0);
	ft_strncpy(res[*i], srt, end - srt);
	(*i)++;
	return (1);
}

char	**ft_split(const char *s, char c)
{
	const char	*start;
	char		**result;
	size_t		index;

	result = malloc((ft_count(s, c) + 1) * sizeof(char *));
	if (!s || !result)
		return (free(result), NULL);
	start = s;
	index = 0;
	while (*s)
	{
		if (*s == c)
		{
			if (s > start && !ft_allocate(result, start, s, &index))
				return (ft_free(result, index), NULL);
			start = s + 1;
		}
		if (*s++ == '\'')
			while (*s && *s != '\'')
				s++;
	}
	if (s > start && !ft_allocate(result, start, s, &index))
		return (ft_free(result, index), NULL);
	result[index] = NULL;
	return (result);
}

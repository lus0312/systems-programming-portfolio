#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# define OPEN_MAX 1024
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);

char	*read_line(int fd, char *buffer, char **backup);

char	*extract_line(int fd, char **backup);

char	*update_backup(int fd, char **backup);

char	*gnl_strjoin(char *s1, char *s2);

char	*ft_substr(char const *s, unsigned int start, size_t len);

char	*ft_strchr(const char *str, int c);

size_t	ft_strlcpy(char *dst, const char *src, size_t size);

size_t	ft_strlen(const char *str);

#endif

#include "libft.h"

char    *ft_substr(char const *s, unsigned int start, size_t len)
{
    char	*sub;
    size_t	i;

    if (!s)
        return (NULL);
    if (ft_strlen(s) < start)
        return (ft_strdup(""));
    if (ft_strlen(s) < start + len)
        len = ft_strlen(s) - start;
    sub = (char *)malloc(sizeof(char) * (len + 1));
    if (!sub)
        return (NULL);
    i = 0;
    while (i < len && s[start + i])
    {
        sub[i] = s[start + i];
        i++;
    }
    sub[i] = '\0';
    return (sub);
}
#include "libft.h"

size_t ft_count_words(const char *s, char c)
{
    size_t	i;
    size_t	count;

    i = 0;
    count = 0;
    while (s[i])
    {
        if (s[i] != c)
        {
            count++;
            while (s[i] && s[i] != c)
                i++;
        }
        else
            i++;
    }
    return (count);
}

static char	**ft_free_arr(char **arr, size_t i)
{
    while (i > 0)
    {
        free(arr[i]);
        i--;
    }
    free(arr);
    return (NULL);
}

char	**ft_split(char const *s, char c)
{
    char	**arr;
    size_t	i;
    size_t	j;
    size_t	k;

    if (!s)
        return (NULL);
    arr = (char **)malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
    if (!arr)
        return (NULL);
    i = 0;
    j = 0;
    while (i < ft_count_words(s, c))
    {
        while (s[j] == c)
            j++;
        k = j;
        while (s[j] && s[j] != c)
            j++;
        arr[i] = ft_substr(s, k, j - k);
        if (!arr[i])
            return (ft_free_arr(arr, i));
        i++;
    }
    arr[i] = NULL;
    return (arr);
}
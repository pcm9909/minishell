#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t len)
{
    unsigned char	*dest;
    size_t			i;

    dest = (unsigned char *)ptr;
    i = 0;
    while (i < len)
    {
        dest[i] = (unsigned char)value;
        i++;
    }
    return (ptr);
}
#include "libft.h"

void* ft_memmove(void *dst, const void *src, size_t len)
{
    unsigned char *d;
    const unsigned char *s;

    if (dst == NULL && src == NULL)
        return (NULL);

    d = (unsigned char *)dst;
    s = (const unsigned char *)src;
    if (s < d)
    {
        s += len;
        d += len;
        while (len--)
            *(--d) = *(--s);
    }
    else
        while (len--)
            *(d++) = *(s++);
    return (dst);
}
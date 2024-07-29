#include "libft.h"

size_t ft_strlcat(char *dst, const char *src, size_t size)
{
    size_t i;
    size_t j;
    size_t dst_len;
    size_t src_len;

    if (!dst || !src)
        return (0);

    i = 0;
    j = 0;
    dst_len = ft_strlen(dst);
    src_len = ft_strlen(src);
    if (size <= dst_len)
        return (size + src_len); // 이만큼의 공간이 있어야 복사가 가능함을 알려주는 것
    while (dst[i])
        i++;
    while (src[j] && i < size - 1)
        dst[i++] = src[j++];
    if (i < size)
        dst[i] = '\0';
    return (dst_len + src_len); // 복사를 성공했음
}

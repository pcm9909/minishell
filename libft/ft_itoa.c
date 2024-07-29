#include "libft.h"

char *ft_itoa(int n)
{
    char *str;
    int len;
    int sign;
    int temp;

    len = 1;
    sign = 1;
    if (n < 0)
    {
        sign = -1;
        len++;
    }
    temp = n;
    while (temp /= 10)
        len++;
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (str == NULL)
        return (NULL);
    str[len] = '\0';
    while (len--)
    {
        str[len] = n % 10 * sign + '0';
        n /= 10;
    }
    if (sign == -1)
        str[0] = '-';
    return (str);
}
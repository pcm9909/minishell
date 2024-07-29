#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
    t_list	*new;
    t_list	*temp;

    if (!lst || !f)
        return (NULL);
    new = ft_lstnew(f(lst->content));
    if (!new)
        return (NULL);
    temp = new;
    lst = lst->next;
    while (lst)
    {
        temp->next = ft_lstnew(f(lst->content));
        if (!temp->next)
        {
            ft_lstclear(&new, del);
            return (NULL);
        }
        temp = temp->next;
        lst = lst->next;
    }
    return (new);
}
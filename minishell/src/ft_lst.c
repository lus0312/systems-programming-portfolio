#include "minishell.h"

int	ft_lstsize(t_list *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}

t_list	*ft_lstlast(t_list *lst)
{
	if (lst)
	{
		while (lst->next)
			lst = lst->next;
	}
	return (lst);
}

void	ft_lstclear(t_list **lst)
{
	t_list	*temp;
	t_list	*next;

	if (!lst)
		return ;
	temp = *lst;
	while (temp)
	{
		next = temp->next;
		if (temp->data)
			free(temp->data);
		if (temp->fd)
		{
			close(temp->fd);
			temp->fd = 0;
		}
		free(temp);
		temp = next;
	}
	*lst = NULL;
}

t_list	*ft_lstadd_back(t_list **lst, char *data, int role)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
	{
		ft_lstclear(lst);
		return (NULL);
	}
	new->data = data;
	new->role = role;
	new->next = NULL;
	new->redir = REDIR_NONE;
	new->fd = 0;
	if (*lst)
		ft_lstlast(*lst)->next = new;
	else
		*lst = new;
	return (*lst);
}

char	*ft_lsttostr(t_list **lst)
{
	char	*tmp;
	char	*rtn;
	t_list	*temp;

	rtn = ft_strdup((*lst)->data);
	if (!rtn)
		return (NULL);
	temp = (*lst)->next;
	while (temp)
	{
		tmp = rtn;
		rtn = ft_straddjoin(tmp, temp->data, 0);
		if (!rtn)
			return (ft_lstclear(lst), free(tmp), NULL);
		free(tmp);
		tmp = NULL;
		temp = temp->next;
	}
	return (rtn);
}

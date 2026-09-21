#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

//Stack
typedef struct s_list
{
	struct s_list	*next;
	long			value;
	int				rank;

}	t_list;

typedef struct s_stack
{
	t_list	*a;
	t_list	*b;
}	t_stack;

//linked_list
t_list	*ft_lstnew(long value);
void	ft_lstadd_front(t_list **stack, t_list *new);
void	ft_lstadd_back(t_list **stack, t_list *new);
t_list	*ft_lstlast(t_list *stack);
//void	ft_lstdelone(t_list **stack);
int		ft_lstsize(t_list *stack);

//check_args
void	parse_args_to_stack(int argc, char **argv, t_list **a);
char	*remove_quotes(const char *s, char c);
int		is_sorted(t_list *a);
int		is_empty_string(const char *str);
int		ft_isdigit(char *argv);
int		range_and_dup_check(t_list *a, long num);
void	handle_empty_string(char **values, t_list **a);

//ranking
void	rank_stack_nodes(t_list **stack);

//simple_version
void	simple_sort(t_list **a, t_list **b, int size);

//radix_sort
int		get_max_bit_len(t_list *a, int size);
void	radix_sort(t_list **a, t_list **b, int size);

//commands
void	pa(t_list **stack_a, t_list **stack_b);
void	pb(t_list **stack_a, t_list **stack_b);
void	ra(t_list **stack_a);
void	rb(t_list **stack_b);
void	rra(t_list **stack_a);
void	rrb(t_list **stack_b);
void	sa(t_list *stack_a);
void	sb(t_list *stack_b);
void	ss(t_list *stack_a, t_list *stack_b);

//libft

long	ft_atoi(const char *str);
int		ft_strlen(const char *str);
char	**ft_split(char const *s, char c);
int		ft_strchr(const char *str, char c);
void	ft_putendl_fd(char *s, int fd);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *string);
//error_and_free

void	print_error(void);
void	ft_free_split(char **split);
void	free_stack(t_list **stack);

#endif

#include "minishell.h"

volatile sig_atomic_t	g_sig_status = 0;

static t_list	*ft_set_msh_env(char **envp, t_list **env)
{
	int		i;
	int		len;
	char	*temp;

	i = 0;
	while (envp[i] != NULL)
	{
		len = ft_strlen(envp[i]);
		temp = ft_strndup(envp[i], len);
		if (!temp)
		{
			write(STDERR_FILENO, \
			"msh: Error!: fail to required setting\n", 39);
			if (*env)
				ft_lstclear(env);
			return (NULL);
		}
		*env = ft_lstadd_back(env, temp, ENV);
		i++;
	}
	return (*env);
}

static int	ft_isempty(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\0')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_get_readline(char **line, t_list **env, int *status)
{
	if ((signal(SIGINT, ft_signal_int_handler) == SIG_ERR) \
	|| (signal(SIGQUIT, SIG_IGN) == SIG_ERR))
		return (1);
	while (1)
	{
		g_sig_status = 0;
		*line = readline("minishell: ");
		if (g_sig_status == 130)
			*status = 130;
		if (!*line)
			break ;
		if (ft_isempty(*line))
		{
			free(*line);
			continue ;
		}
		if (ft_strlen(*line))
			add_history(*line);
		if (ft_run_minishell(line, env, status))
			break ;
	}
	return (0);
}

static int	ft_set_minishell(int ac, char **av, t_list **env, char **envp)
{
	if (ac != 1 || !av[0])
	{
		printf("msh: This program dose not take arguments.\n");
		return (1);
	}
	if (!*env)
	{
		*env = ft_set_msh_env(envp, env);
		if (!*env)
			return (1);
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	int		status;
	char	*line;
	t_list	*env;

	env = NULL;
	line = NULL;
	status = 0;
	if (ft_set_minishell(ac, av, &env, envp))
		return (1);
	if (ft_turn_off_echo())
		return (1);
	if (ft_get_readline(&line, &env, &status))
		printf("msh: Error!: fail to run minishell\n");
	rl_clear_history();
	if (ft_turn_on_echo())
		return (1);
	ft_lstclear(&env);
	printf("exit\n");
	return (status);
}

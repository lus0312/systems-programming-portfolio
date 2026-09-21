#include "minishell.h"

void	ft_signal_int_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (g_sig_status)
		{
			printf("\n");
			g_sig_status = -1;
		}
		else
		{
			g_sig_status = 130;
			printf("minishell: \n");
			rl_on_new_line();
			rl_replace_line("", 1);
			rl_redisplay();
		}
	}
}

void	ft_signal_quit_handler(int signum)
{
	if (signum == SIGQUIT)
	{
		if (g_sig_status)
		{
			printf("^\\");
			g_sig_status = -1;
		}
		else
		{
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

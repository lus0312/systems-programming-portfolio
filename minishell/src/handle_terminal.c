#include "minishell.h"

int	ft_turn_off_echo(void)
{
	struct termios	handle_termios;

	if (tcgetattr(STDIN_FILENO, &handle_termios) == -1)
	{
		perror("msh: tcgetattr failed");
		return (1);
	}
	handle_termios.c_cc[VQUIT] = _POSIX_VDISABLE;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &handle_termios) == -1)
	{
		perror("msh: tcsetattr failed");
		return (1);
	}
	return (0);
}

int	ft_turn_on_echo(void)
{
	struct termios	handle_termios;

	if (tcgetattr(STDIN_FILENO, &handle_termios) == -1)
	{
		perror("msh: tcgetattr failed");
		return (1);
	}
	handle_termios.c_cc[VQUIT] = 0x1c;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &handle_termios) == -1)
	{
		perror("msh: tcsetattr failed");
		return (1);
	}
	return (0);
}

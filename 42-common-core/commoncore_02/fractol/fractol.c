#include "fractol.h"

void	malloc_error(void)
{
	perror("Problems with malloc");
	exit(EXIT_FAILURE);
}

void	free_fractol(t_fractol *fractol)
{
	if (fractol->img.img_ptr)
	{
		mlx_destroy_image(fractol->mlx_connection, fractol->img.img_ptr);
		// fractol->img.img_ptr = NULL;
	}
	if (fractol->mlx_window)
	{
		mlx_destroy_window(fractol->mlx_connection, fractol->mlx_window);
		// fractol->mlx_window = NULL;
	}
	if (fractol->mlx_connection)
	{
		mlx_destroy_display(fractol->mlx_connection);
		// free(fractol->mlx_connection);
		// fractol->mlx_connection = NULL;
	}
	if (fractol->color_table)
	{
		free(fractol->color_table);
		fractol->color_table = NULL;
	}
}

void	fractol_parse(t_fractol *fractol, int ac, char **av)
{
	if (ac == 2 && !ft_strncmp(av[1], "mandelbrot", 10))
	{
		fractol->name = av[1];
	}
	else if (ac == 4 && !ft_strncmp(av[1], "julia", 5))
	{
		fractol->name = av[1];
		fractol->julia_x = atodbl(av[2]);
		fractol->julia_y = atodbl(av[3]);
		if ((fractol->julia_x) >= 2 || (fractol->julia_y) >= 2)
		{
			ft_putendl_fd(ERROR_MESSAGE2, STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		ft_putendl_fd(ERROR_MESSAGE, STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

int	main(int ac, char *av[])
{
	t_fractol	fractol;

	fractol_parse(&fractol, ac, av);
	fractol_init(&fractol);
	fractol_render(&fractol);
	mlx_loop(fractol.mlx_connection);
	return (0);
}

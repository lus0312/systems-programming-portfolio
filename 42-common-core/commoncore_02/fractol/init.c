#include "fractol.h"

double	map(double unscaled_num, double new_min, double new_max)
{
	double	old_min;
	double	old_max;

	old_min = 0;
	old_max = 42;
	return ((new_max - new_min) * (unscaled_num - old_min)
		/ (old_max - old_min) + new_min);
}

void	data_init(t_fractol *fractol)
{
	int	i;

	fractol->escape_value = 4;
	fractol->iterations_definition = 42;
	fractol->zoom = 1.0;
	fractol->color_table = malloc(sizeof(int) * fractol->iterations_definition);
	fractol->offset_x = -2.0;
	fractol->offset_y = -2.0;
	if (!fractol->color_table)
		malloc_error();
	i = 0;
	while (i < fractol->iterations_definition)
	{
		fractol->color_table[i] = map(i, COLOR_BLACK, COLOR_WHITE);
		i++;
	}
}

void	events_init(t_fractol *f)
{
	mlx_hook(f->mlx_window, KeyPress, KeyPressMask, key_handler, f);
	mlx_mouse_hook(f->mlx_window, mouse_handler, f);
	mlx_hook(f->mlx_window, DestroyNotify,
		StructureNotifyMask, close_handler, f);
}

void	fractol_init(t_fractol *f)
{
	f->mlx_connection = mlx_init();
	f->mlx_window = mlx_new_window(f->mlx_connection, WIDTH, HEIGHT, f->name);
	f->img.img_ptr = mlx_new_image(f->mlx_connection, WIDTH, HEIGHT);
	f->img.pixels_ptr = mlx_get_data_addr(f->img.img_ptr, &f->img.bpp,
			&f->img.line_len, &f->img.endian);
	events_init(f);
	data_init(f);
}

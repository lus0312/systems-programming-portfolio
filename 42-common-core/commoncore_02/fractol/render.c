#include "fractol.h"

void	set_complex_values(t_fractol *fractol, int x, int y)
{
	if (!ft_strncmp(fractol->name, "julia", 5))
	{
		fractol->z.x = (x * fractol->scale_x) + fractol->offset_x;
		fractol->z.y = ((HEIGHT - y) * fractol->scale_y) + fractol->offset_y;
		fractol->c.x = fractol->julia_x;
		fractol->c.y = fractol->julia_y;
	}
	else
	{
		fractol->z.x = 0.0;
		fractol->z.y = 0.0;
		fractol->c.x = x * fractol->scale_x + fractol->offset_x;
		fractol->c.y = (HEIGHT - y) * fractol->scale_y + fractol->offset_y;
	}
}

void	handle_pixel(int x, int y, t_fractol *fractol)
{
	int	i;
	int	color;
	int	offset;

	i = 0;
	set_complex_values(fractol, x, y);
	while (i < fractol->iterations_definition)
	{
		fractol->zx2 = fractol->z.x * fractol->z.x;
		fractol->zy2 = fractol->z.y * fractol->z.y;
		fractol->zxzy = 2 * fractol->z.x * fractol->z.y;
		fractol->z.x = fractol->zx2 - fractol->zy2 + fractol->c.x;
		fractol->z.y = fractol->zxzy + fractol->c.y;
		if (fractol->zx2 + fractol->zy2 > fractol->escape_value)
		{
			color = fractol->color_table[i];
			offset = (y * fractol->img.line_len) + (x * (fractol->img.bpp / 8));
			*(unsigned int *)(fractol->img.pixels_ptr + offset) = color;
			return ;
		}
		i++;
	}
	offset = (y * fractol->img.line_len) + (x * (fractol->img.bpp / 8));
	*(unsigned int *)(fractol->img.pixels_ptr + offset) = COLOR_CYAN;
}

void	fractol_render(t_fractol *f)
{
	int	x;
	int	y;

	f->scale_x = (4.0 / WIDTH) / f->zoom;
	f->scale_y = (4.0 / HEIGHT) / f->zoom;
	y = -1;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
			handle_pixel(x, y, f);
	}
	mlx_put_image_to_window(f->mlx_connection, f->mlx_window,
		f->img.img_ptr, 0, 0);
}

#include "fractol.h" 

int	close_handler(void *param)
{
	t_fractol	*fractol;

	fractol = (t_fractol *)param;
	free_fractol(fractol);
	exit(EXIT_SUCCESS);
}

int	key_handler(int keysym, t_fractol *fractol)
{
	if (keysym == XK_Escape)
		close_handler(fractol);
	return (0);
}

int	mouse_handler(int button, int x, int y, t_fractol *f)
{
	double	mouse_re;
	double	mouse_im;

	mouse_re = (x * (4.0 / WIDTH) / f->zoom) + f->offset_x;
	mouse_im = ((HEIGHT - y) * (4.0 / HEIGHT) / f->zoom) + f->offset_y;
	if (button == 4)
		f->zoom *= 1.15;
	else if (button == 5)
		f->zoom *= 0.85;
	else
		return (0);
	f->offset_x = mouse_re - (x * (4.0 / WIDTH) / f->zoom);
	f->offset_y = mouse_im - ((HEIGHT - y) * (4.0 / HEIGHT) / f->zoom);
	fractol_render(f);
	return (0);
}

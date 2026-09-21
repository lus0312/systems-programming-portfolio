#ifndef FRACTOL_H
# define FRACTOL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <X11/X.h>
# include <X11/Xlib.h>
# include <X11/keysym.h>
# include "minilibx-linux/mlx.h"

# define ERROR_MESSAGE "Usage: ./fractol [mandelbrot | julia <v1> <v2>]"
# define ERROR_MESSAGE2 "USAGE: v1 and v2 must be floats less than 2"
# define WIDTH	600
# define HEIGHT	600
# define COLOR_BLACK    0x000000  // Black color (RGB: 0, 0, 0)
# define COLOR_WHITE    0xFFFFFF  // White color (RGB: 255, 255, 255)
# define COLOR_CYAN     0x00FFFF  // Cyan color (RGB: 0, 255, 255)

typedef struct s_complex
{
	double	x;
	double	y;
}	t_complex;

typedef struct s_img
{
	void	*img_ptr;
	char	*pixels_ptr;
	int		bpp;
	int		endian;
	int		line_len;
}	t_img;

typedef struct s_fractol
{
	char		*name;
	void		*mlx_connection;
	void		*mlx_window;
	t_img		img;
	double		escape_value;
	int			iterations_definition;
	int			*color_table;
	double		julia_x;
	double		julia_y;
	double		offset_x;
	double		offset_y;
	double		zoom;
	double		scale_x;
	double		scale_y;
	double		zx2;
	double		zy2;
	double		zxzy;
	t_complex	z;
	t_complex	c;
}	t_fractol;

int			ft_strncmp(const char *s1, const char *s2, size_t num);
void		ft_putendl_fd(char *s, int fd);
double		atodbl(char *s);
void		fractol_init(t_fractol *fractol);
void		fractol_render(t_fractol *fractol);
double		map(double unscaled_num, double new_min, double new_max);
int			key_handler(int keysym, t_fractol *fractol);
void		malloc_error(void);
int			close_handler(void *param);
int			mouse_handler(int button, int x, int y, t_fractol *fractol);
void		free_fractol(t_fractol *fractol);

#endif

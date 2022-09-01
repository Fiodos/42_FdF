/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:48:24 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/09/01 19:44:17 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "lib/fdf.h"
#include "../libft/libft.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

t_vec	*create_frame_new(int x_c, int y_c, int fd)
{
	t_vec	*vec; //head of the list;
	char	*line;
	char	**ps;
	int		i;
	int		z;

	vec = new_vec(0, 0, 0);
	line = get_next_line(fd);
	while (line)
	{
		i = 0;
		ps = ft_split(line, ' ');
		while (ps[i])
		{
			z = ft_atoi(ps[i]);
			if (z > 0)
				z = z + 20;
			lstadd_back(&vec, new_vec(x_c, y_c, z));
			x_c += 30;
			i++;
		}
		x_c = x_c - (i * 30); // set x back to the initial value;
		y_c += 30;
		line = get_next_line(fd);
	}
	return (vec);
}

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	bresenham_x(t_data *img, int x, int y, int x2, int y2) // if slope < 1;
{
	int	dx;
	int	dy;
	int	p;

	dx = x2 - x;
	if (dx < 0)
		dx = -(dx);
	dy = y2 - y;
	if (dy < 0)
		dy = -(dy);
	p = (2 * dy) - dx;
	while (x <= x2)
	{
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
		x++;
		if (p <= 0)
			p = p + 2 * dy;
		else
		{
			p = p + (2 * dy) - (2 * dx);
			y++;
		}
	}
}

void	bresenham_y(t_data *img, int x, int y, int x2, int y2) // if slope > 1;
{
	int	dx;
	int	dy;
	int	p;

	dx = x2 - x;
	if (dx < 0)
		dx = -(dx);
	dy = y2 - y;
	if (dy < 0)
		dy = -(dy);
	p = (2 * dy) - dx;
	while (y <= y2)
	{
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
		y--;
		if (p < 0)
			p = p + 2 * dy;
		else
		{
			p = p + (2 * dy) - (2 * dx);
			x++;
		}
	}
}

void	bresenham_y_new(t_data *img, int x, int y, int x2, int y2) // if slope > 1; or < 1?
{
	int	dx;
	int	dy;
	int	p;

	dx = x2 - x;
	dy = y2 - y;
	if (dy < 0)
		dy = -(dy);
	if (dx < 0)
		dx = -(dx);
	p = (2 * dy) - dx;
	while (x <= x2)
	{
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
		x++;
		if (p <= 0)
			p = p + 2 * dy;
		else
		{
			p = p + (2 * dy) - (2 * dx);
			y--;
		}
	}
}

void	new_bresen(t_data *img, int x1, int y1, int x2, int y2) // if slope < 1;
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int p = (2 * dx) - dy;

	while (x1 <= x2)
	{
		my_mlx_pixel_put(img, x1, y1, 0x00FF0000);
		x1++;
		if (p < 0)
			p = p + (2 * dy);
		else
		{
			p = p + (2 * dy) - (2 * dx);
			y1++;
		}
	}
}

void	new_bresen_2(t_data *img, int x1, int y1, int x2, int y2) // if slope > 1;
{
	int dx = x2 - x1;
	if (dx < 0)
		dx = -dx;
	int dy = y2 - y1;
	if (dy < 0)
		dy = -dy;
	int p = (2 * dx) - dy;

	while (y1 <= y2)
	{
		my_mlx_pixel_put(img, x1, y1, 0xFFFFFF);
		y1++;
		if (p < 0)
			p = p + (2 * dy);
		else
		{
			p = p + (2 * dy) - (2 * dx);
			x1++;
		}
	}
}

void	bresenham(t_vec *vec, t_vec *next, t_data *img)
{
	int	delta_x;
	int	delta_y;
	int	i;

	delta_x = next->x - vec->x;
	delta_y = next->y - vec->y;
	i = 0;
	if (delta_x == 0)
		return ;
	if ((delta_y / delta_x) > 1) // slope > 1;
		bresenham_y_new(img, vec->x, vec->y, next->x, next->y);
	else
		bresenham_x(img, vec->x, vec->y, next->x, next->y);
}

void	rotate(t_vec *vec, int x, int y)
{
	int x_new;
	int y_new;

	x_new = (vec->x - x) * cos(PI/4) - (vec->y - y) * sin(PI/4);
	y_new = (vec->x - x) * sin(PI/4) + (vec->y - y) * cos(PI/4);
	vec->x = x_new + x;
	vec->y = y_new + y;
}

void	rotate_around_x(t_vec *vec)
{
	int	y_new;
	int	z_new;

	y_new = (vec->y * cos(PI/4)) - (vec->z * sin(PI/4));
	z_new = (vec->y * sin(PI/4) + (vec->z) * cos(PI/4));
	vec->y = y_new;
	vec->z = z_new;
}

t_vec	*create_frame(int x_c, int y_c, int length, int width)
{
	int		x;
	int		y;
	int		i;
	int		j;
	t_vec	*vec;
	t_vec	*n_vec;

	// where does the 30 come from? this is a way to hardcode the size of the frame;

	// x and y are the coordinates for the very first vector in our frame (top left);
	x = x_c - ((length * 30) / 2);
	y = y_c - ((width * 30)/ 2);
	j = 0;
	vec = new_vec(x, y, 0); // here the vertices are initialized;
	while (j < width)
	{
		i = 0;
		while (i < length)
		{
			x += 30;
			n_vec = new_vec(x, y, 0);
			lstadd_back(&vec, n_vec);
			i++;
		}
		x = x - (i * 30);
		y += 30;
		j++;
	}
	return (vec);
}

int	main(int argc, char **argv)
{
	// int		fd = open(argv[argc - 1], O_RDONLY);
	void	*mlx;
	void	*mlx_win;
	t_data	img;
	t_vec	*vec;
	t_vec	*next_vec;

	argv = NULL;
	argc = 0;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "FdF");
	img.img = mlx_new_image(mlx, 1920, 1080);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								&img.endian);
	int x_c = 1920 / 2; // c = center;
	int y_c = 1080 / 2;
	// vec = create_frame_new(x_c, y_c, fd);
	vec = create_frame(x_c, y_c, 18, 11);
	lstiter(vec, &rotate, x_c, y_c);
	// lstiter_rx(vec, &rotate_around_x);
	vec = vec->next;
	next_vec = vec->next;
	int k = 0;
	int c = 190;
	t_vec *test = vec;
	while (next_vec)
	{
		// if ((next_vec->y - vec->y) - (next_vec->x - vec->x) <= 1)
		// {
		// 	new_bresen(&img, vec->x, vec->y, next_vec->x, next_vec->y);
		// }
		// else
		// {
		// 	new_bresen_2(&img, vec->x, vec->y, next_vec->x, next_vec->y);
		// }
		my_mlx_pixel_put(&img, vec->x, vec->y, 0x00FF0000);
		vec = vec->next;
		next_vec = vec->next;
		k++;
		if (k == c)
		{
			bresenham_y_new(&img, vec->x, vec->y, test->x, test->y);
			test = test->next;
			c++;
		}
	}
	my_mlx_pixel_put(&img, vec->x, vec->y, 0x00FF0000);
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
}

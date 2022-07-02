/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:48:24 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/07/02 18:13:58 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "lib/fdf.h"
#include "../libft/libft.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

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
	dy = y2 - y;
	p = (2 * dy) - dx;
	while (x <= x2)
	{
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
		x++;
		if (p < 0)
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
	dy = y2 - y;
	p = (2 * dy) - dx;
	while (y <= y2)
	{
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
		y++;
		if (p < 0)
			p = p + 2 * dy;
		else
		{
			p = p + (2 * dy) - (2 * dx);
			x--;
		}
	}
}

void	bresenham(t_vec *vec, t_vec *next, t_data img)
{
	int	delta_x;
	int	delta_y;
	int	control;
	int	i;

	delta_x = next->x - vec->x;
	delta_y = next->y - vec->y;
	control = 18; // length;
	i = 0;
	if (delta_x == 0)
		return ;
	// if ((delta_y / delta_x) > 1)
		// bresenham_y(&img, vec->x, vec->y, next->x, next->y);
	// else
		bresenham_x(&img, vec->x, vec->y, next->x, next->y);
	// while (i < control && next->next != NULL)
	// {
	// 	next = next->next;
	// 	i++;
	// }
	// // printf("%d %d\n", next->x, next->y);
	// if ((delta_y / delta_x) > 1)
	// 	bresenham_y(&img, vec->x, vec->y, next->x, next->y);
	// else
	// 	bresenham_x(&img, vec->x, vec->y, next->x, next->y);
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

	x = x_c - ((length * 30) / 2);
	y = y_c - ((width * 30)/ 2);
	j = 0;
	vec = new_vec(x, y, 0);
	while (j < width)
	{
		i = 0;
		while (i < length)
		{
			x += 30;
			n_vec = new_vec(x, y, 0);
			lstadd_back(&vec, n_vec);
			// my_mlx_pixel_put(&img, n_vec->x, n_vec->y, 0x00FF0000);
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
	int	i = 0; // y;
	int	j = 0; // x;
	int		fd = open(argv[argc - 1], O_RDONLY);
	char	*line;
	char	**ps;
	void	*mlx;
	void	*mlx_win;
	t_data	img;
	t_vec	*vec;
	t_vec	*next_vec;

	line = get_next_line(fd);
	ps = ft_split(line, ' ');
	while (ps[j])
		j++;
	while (line)
	{
		ft_printf("%s", line);
		line = get_next_line(fd);
		i++;
	}
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "FdF");
	img.img = mlx_new_image(mlx, 1920, 1080);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								&img.endian);
	int x_c = 1920 / 2;
	int y_c = 1080 / 2;
	vec = create_frame(x_c, y_c, j, i);
	lstiter(vec, &rotate, x_c, y_c);
	lstiter_rx(vec, &rotate_around_x);
	vec = vec->next;
	next_vec = vec->next;
	int test = vec->x;
	int test2 = vec->y;
	int k = 0;
	while (next_vec)
	{
		bresenham(vec, next_vec, img);
		my_mlx_pixel_put(&img, vec->x, vec->y, 0x00FF0000);
		vec = vec->next;
		next_vec = vec->next;
		k++;
	}
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
}

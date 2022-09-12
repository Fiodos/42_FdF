/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:48:24 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/09/12 17:58:28 by fyuzhyk          ###   ########.fr       */
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

void	bresen_test(t_data *img, int x1, int y1, int x2, int y2) {
	int	dx;
	int dy;
	int absdx;
	int absdy;
	int d;
	int i;

	dx = x2 - x1;
	dy = y2 - y1;
	if (dx < 0)
		absdx = -dx;
	else
		absdx = dx;
	if (dy < 0)
		absdy = -dy;
	else
		absdy = dy;

	if (absdx > absdy) { // slope < 1;
		i = 0;
		d = 2 * absdy - absdx;
		while (i < absdx)
		{
			if (dx < 0)
				x1 = x1 - 1;
			else
				x1 = x1 + 1;
			if (d < 0)
				d = d + 2 * absdy;
			else
			{
				if (dy < 0)
					y1 = y1 - 1;
				else
					y1 = y1 + 1;
				d = d + (2 * absdy - 2 * absdx);
			}
			my_mlx_pixel_put(img, x1, y1, 0x00FF0000);
			i++;
		}
	}
	else // slope >= 1;
	{
		i = 0;
		d = 2 * absdx - absdy;
		while (i < absdy)
		{
			if (dy < 0)
				y1 = y1 - 1;
			else
				y1 = y1 + 1;
			if (d < 0)
				d = d + 2 * absdx;
			else
			{
				if (dx < 0)
					x1 = x1 - 1;
				else
					x1 = x1 + 1;
				d = d + 2 * absdx - 2 * absdy;
			}
			my_mlx_pixel_put(img, x1, y1, 0x00FF0000);
			i++;
		}
	}

}

t_vec	*create_frame_new(int x_c, int y_c, int fd, int *length)
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
				z = z + 10;
			if (z < 0)
				z = z - 10;
			lstadd_back(&vec, new_vec(x_c, y_c, z));
			x_c += 10;
			i++;
		}
		*length = i;
		x_c = x_c - (i * 10); // set x back to the initial value;
		y_c += 10;
		line = get_next_line(fd);
	}
	return (vec);
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

void	draw_to_next_row(t_vec *vector, t_data *img, int length)
{
	int i = 0;
	t_vec *next_vec;

	next_vec = vector;
	while (i < length)
	{
		next_vec = next_vec->next;
		if (next_vec == NULL)
			return ;
		i++;
	}
	bresen_test(img, vector->x, vector->y, next_vec->x, next_vec->y);
}

int	main(int argc, char **argv)
{
	int		fd = open(argv[argc - 1], O_RDONLY);
	void	*mlx;
	void	*mlx_win;
	t_data	img;
	t_vec	*vec;
	t_vec	*next_vec;
	t_vec 	*head;
	int i = 0;
	int length = 0;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "FdF");
	img.img = mlx_new_image(mlx, 1920, 1080);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								&img.endian);
	int x_c = 1920 / 2;
	int y_c = 1080 / 2;
	vec = create_frame_new(x_c, y_c, fd, &length);
	lstiter(vec, &rotate, x_c, y_c);
	lstiter_rx(vec, &rotate_around_x);
	head = vec;
	vec = vec->next;
	next_vec = vec->next;
	while (next_vec)
	{
		bresen_test(&img, vec->x, vec->y, next_vec->x, next_vec->y);
		vec = vec->next;
		next_vec = vec->next;
		i++;
		int t = i % (length - 1);
		if (!t && next_vec != NULL)
		{
			vec = vec->next;
			next_vec = vec->next;
		}
	}
	vec = head->next;
	next_vec = vec->next;
	while(next_vec)
	{
		draw_to_next_row(vec, &img, length);
		vec = vec->next;
		next_vec = vec->next;
	}
	my_mlx_pixel_put(&img, vec->x, vec->y, 0x00FF0000);
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
}

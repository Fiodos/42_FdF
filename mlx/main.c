/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:48:24 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/09/13 19:11:48 by fyuzhyk          ###   ########.fr       */
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

typedef struct	s_vars {
	void	*mlx;
	void	*win;
	t_data	img;
	int		scale;
	int		length;
	int		width;
	int		fd;
	t_vec 	*head;
}				t_vars;

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

t_vec	*create_frame_new(int fd, t_vars *vars)
{
	t_vec	*vec; //head of the list;
	char	*line;
	char	**ps;
	int		i;
	int		z;
	int		x_c = 0;
	int		y_c = 0;

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
				z = z + vars->scale;
			if (z < 0)
				z = z - vars->scale;
			lstadd_back(&vec, new_vec(x_c, y_c, z));
			x_c += vars->scale;
			i++;
		}
		vars->length = i;
		x_c = x_c - (i * vars->scale); // set x back to the initial value;
		y_c += vars->scale;
		line = get_next_line(fd);
		vars->width++;
	}
	return (vec);
}

t_vec	*create_frame_zoom(t_vars *vars)
{
	int i = 0;
	int j = 0;
	int x = 0;
	int y = 0;
	t_vec *tmp;
	t_vec *vec;

	tmp = vars->head;
	vec = new_vec(0, 0, 0);
	while (j < vars->width)
	{
		while (i < vars->length)
		{
			// if (tmp->z > 0)
			// 	tmp->z = tmp->z + 1;
			// if (tmp->z < 0)
			// 	tmp->z = tmp->z - 1;
			if (tmp->z > 0)
				tmp->z = tmp->z + 1;
			lstadd_back(&vec, new_vec(x, y, tmp->z));
			tmp = tmp->next;
			x += vars->scale;
			i++;
		}
		x = x - (i * vars->scale); // set x back to the initial value;
		y += vars->scale;
		i = 0;
		j++;
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

t_vec *create_frame(int fd, t_vars *vars)
{
	t_vec *vec;

	vec = malloc(sizeof(t_vec));
	vec = create_frame_new(fd, vars);
	lstiter(vec, &rotate);
	lstiter_rx(vec, &rotate_around_x);
	return (vec);
}

t_vec	*draw_map(t_vars *vars, int fd)
{
	t_vec	*vec;
	t_vec	*next_vec;
	t_vec 	*head;
	int i = 0;

	vec = create_frame(fd, vars); // seems to work fine here;
	head = vec;
	vec = vec->next;
	next_vec = vec->next;
	while (next_vec)
	{
		bresen_test(&vars->img, vec->x, vec->y, next_vec->x, next_vec->y);
		vec = vec->next;
		next_vec = vec->next;
		i++;
		int t = i % (vars->length - 1);
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
		draw_to_next_row(vec, &vars->img, vars->length);
		vec = vec->next;
		next_vec = vec->next;
	}
	return (head);
}

void	draw_map_new(t_vec *head, t_vars *vars)
{
	t_vec *vec;
	t_vec *next_vec;
	int i = 0;

	vec = head->next;
	next_vec = vec->next;
	while(next_vec)
	{
		// vec->x++;
		bresen_test(&vars->img, vec->x, vec->y, next_vec->x, next_vec->y);
		vec = vec->next;
		next_vec = vec->next;
		i++;
		int t = i % (vars->length - 1);
		if (!t && next_vec != NULL)
		{
			vec->x++;
			vec = vec->next;
			next_vec = vec->next;
		}
 	}
	// vec->x++;
	vec = head->next;
	next_vec = vec->next;
	while(next_vec)
	{
		draw_to_next_row(vec, &vars->img, vars->length);
		vec = vec->next;
		next_vec = vec->next;
	}
}

int	key_hook(int keycode, t_vars *vars)
{
	t_vec *head;

	vars->scale++;
	mlx_destroy_image(vars->mlx, vars->img.img);
	vars->img.img = mlx_new_image(vars->mlx, 6000, 6000);
	// draw_map_new(vars->head, vars);
	head = create_frame_zoom(vars);
	lstiter(head, &rotate);
	lstiter_rx(head, &rotate_around_x);
	draw_map_new(head, vars);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	keycode = 0;
	return (0);
}

int mouse_hook(int mousecode, t_vars *vars)
{
	mlx_destroy_image(vars->mlx, vars->img.img);
	vars->img.img = mlx_new_image(vars->mlx, 6000, 6000);
	draw_map_new(vars->head, vars);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	mousecode = 0;
	return (0);
}

int	main(int argc, char **argv)
{
	int		fd = open(argv[argc - 1], O_RDONLY);
	void	*mlx;
	void	*win;
	// t_data	img;
	t_vars vars;

	mlx = mlx_init();
	vars.mlx = mlx;
	win = mlx_new_window(mlx, 1920, 1080, "FdF");
	vars.win = win;
	// img.img = mlx_new_image(mlx, 6000, 6000);
	vars.img.img = mlx_new_image(mlx, 6000, 6000);
	vars.scale = 30;
	vars.fd = fd;
	vars.length = 0;
	vars.width = 0;

	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length,
								&vars.img.endian);
	vars.head = draw_map(&vars, fd);
	mlx_put_image_to_window(mlx, win, vars.img.img, 0, 0);
	mlx_hook(win, 2, 1L<<0, key_hook, &vars);
	// mlx_mouse_hook(vars.win, mouse_hook, &vars);
	mlx_loop(mlx);
}

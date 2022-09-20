/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:48:24 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/09/20 18:38:26 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "lib/fdf.h"
#include "../libft/libft.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	get_next_nodes(t_vec **vec, t_vec **next_vec)
{
	*vec = (*vec)->next;
	*next_vec = (*vec)->next;
}

void	reset_nodes(t_vec **vec, t_vec **next_vec, t_vars *vars)
{
	*vec = vars->head->next;
	*next_vec = (*vec)->next;
}

void	manage_nodes(t_vec *head, t_vec **vec, t_vec **next_vec, char mode)
{
	if (mode == 'd')
		(*vec)->y = (*vec)->y + 10;
	else if (mode == 'u')
		(*vec)->y = (*vec)->y - 10;
	else if (mode == 'r')
		(*vec)->x = (*vec)->x + 10;
	else if (mode == 'l')
		(*vec)->x = (*vec)->x - 10;
	*vec = head->next;
	*next_vec = (*vec)->next;
}

void	manage_row(t_vec **vec, t_vec **next_vec)
{
	*vec = (*vec)->next;
	*next_vec = (*vec)->next;
}

int	get_dx(t_vec *vec, t_vec *next_vec)
{
	int	dx;

	dx = (int)next_vec->x - (int)vec->x;
	return (dx);
}

int	get_dy(t_vec *vec, t_vec *next_vec)
{
	int	dy;

	dy = (int)next_vec->y - (int)vec->y;
	return (dy);
}

int	get_absdx(int dx)
{
	int	absdx;

	if (dx < 0)
		absdx = -dx;
	else
		absdx = dx;
	return (absdx);
}

int	get_absdy(int dy)
{
	int	absdy;

	if (dy < 0)
		absdy = -dy;
	else
		absdy = dy;
	return (absdy);
}

void	get_d_more_than_one(t_vec *vec, t_vec *next_vec, int *y, int *d)
{
	if (get_dy(vec, next_vec) < 0)
		*y = *y - 1;
	else
		*y = *y + 1;
	*d = *d + (2 * get_absdy(get_dy(vec, next_vec)) - 2 * get_absdx(get_dx(vec, next_vec)));
}

void	get_d_less_than_one(t_vec *vec, t_vec *next_vec, int *x, int *d)
{
	if (get_dx(vec, next_vec) < 0)
		*x = *x - 1;
	else
		*x = *x + 1;
	*d = *d + 2 * get_absdx(get_dx(vec, next_vec)) - 2 * get_absdy(get_dy(vec, next_vec));
}

void	bresen_slope_more_than_one(t_vec *vec, t_vec *next_vec, t_data *img)
{
	int	d;
	int	i;
	int	x;
	int	y;

	d = 2 * get_absdy(get_dy(vec, next_vec)) - get_absdx(get_dx(vec, next_vec));
	i = 0;
	x = (int)vec->x;
	y = (int)vec->y;
	while (i < get_absdx(get_dx(vec, next_vec)))
	{
		if (get_dx(vec, next_vec) < 0)
			x = x - 1;
		else
			x = x + 1;
		if (d < 0)
			d = d + 2 * get_absdy(get_dy(vec, next_vec));
		else
			get_d_more_than_one(vec, next_vec, &y, &d);
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
		i++;
	}
}

void	bresen_slope_less_than_one(t_vec *vec, t_vec *next_vec, t_data *img)
{
	int	d;
	int	i;
	int	x;
	int	y;

	d = 2 * get_absdy(get_dy(vec, next_vec)) - get_absdx(get_dx(vec, next_vec));
	i = 0;
	x = (int)vec->x;
	y = (int)vec->y;
	while (i < get_absdy(get_dy(vec, next_vec)))
	{
		if (get_dy(vec, next_vec) < 0)
			y = y - 1;
		else
			y = y + 1;
		if (d < 0)
			d = d + 2 * get_absdx(get_dx(vec, next_vec));
		else
			get_d_less_than_one(vec, next_vec, &x, &d);
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
		i++;
	}
}

void	bresenham(t_data *img, t_vec *vec, t_vec *next_vec)
{
	int	dx;
	int dy;

	dx = (int)next_vec->x - (int)vec->x;
	dy = (int)next_vec->y - (int)vec->y;
	if (get_absdx(dx) > get_absdy(dy))
		bresen_slope_more_than_one(vec, next_vec, img);
	else
		bresen_slope_less_than_one(vec, next_vec, img);
}

int	get_z(t_vars *vars, char **split)
{
	int	z;

	z = ft_atoi(split[vars->length]);
	if (z <= 15 && z > 0)
		z *= vars->scale;
	else if (z >= -15 && z < 0)
		z *= vars->scale;
	else if (z > 15 || z < -15)
		z *= 2;
	vars->z_values[vars->i_z] = z;
	vars->i_z++;
	return (z);
}


void	init_x_list(t_vec *vec, t_vars *vars)
{
	int i = 0;
	t_vec	*tmp;

	tmp = vec->next;
	while (tmp)
	{
		vars->x_values[i] = tmp->x;
		i++;
		tmp = tmp->next;
	}
}

void	init_y_list(t_vec *vec, t_vars *vars)
{
	int i = 0;
	t_vec	*tmp;

	tmp = vec->next;
	while (tmp)
	{
		vars->y_values[i] = tmp->y;
		i++;
		tmp = tmp->next;
	}
}

void	init_z_list(t_vec *vec, t_vars *vars)
{
	int i = 0;
	t_vec	*tmp;

	tmp = vec->next;
	while (tmp)
	{
		vars->z_values[i] = tmp->z;
		i++;
		tmp = tmp->next;
	}
}

void	init_z_list_dyn(t_vec *vec, t_vars *vars)
{
	int i = 0;
	t_vec	*tmp;

	tmp = vec->next;
	while (tmp)
	{
		vars->z_values_dyn[i] = tmp->z;
		i++;
		tmp = tmp->next;
	}
}

void	init_x_list_dyn(t_vec *vec, t_vars *vars)
{
	int i = 0;
	t_vec	*tmp;

	tmp = vec->next;
	while (tmp)
	{
		vars->x_values_dyn[i] = tmp->x;
		i++;
		tmp = tmp->next;
	}
}

void	init_y_list_dyn(t_vec *vec, t_vars *vars)
{
	int i = 0;
	t_vec	*tmp;

	tmp = vec->next;
	while (tmp)
	{
		vars->y_values_dyn[i] = tmp->y;
		i++;
		tmp = tmp->next;
	}
}

t_vec	*create_frame_new(int fd, t_vars *vars, int x, int y)
{
	char	*line;
	char	**ps;
	t_vec	*vec;

	vec = new_vec(x, y, 0);
	line = get_next_line(fd);
	while (line)
	{
		vars->length = 0;
		ps = ft_split(line, ' ');
		while (ps[vars->length])
		{
			lstadd_back(&vec, new_vec(x, y, get_z(vars, ps)));
			x += vars->scale;
			vars->length++;
		}
		x = x - (vars->length * vars->scale); // set x back to the initial value;
		y += vars->scale;
		line = get_next_line(fd);
	}
	close(fd);
	return (vec);
}

void	rotate(t_vec *vec, int x, int y)
{
	float x_new;
	float y_new;

	x_new = vec->x - x * cos(PI/4) - vec->y - y * sin(PI/4);
	y_new = vec->x - x * sin(PI/4) + vec->y - y * cos(PI/4);
	vec->x = x_new + x;
	vec->y = y_new + y;
}

void	rotate_around_x(t_vec *vec)
{
	float	y_new;
	float	z_new;

	y_new = (vec->y * cos(PI/4)) - (vec->z * sin(PI/4));
	z_new = (vec->y * sin(PI/4) + (vec->z) * cos(PI/4));
	vec->y = y_new;
	vec->z = z_new;
}

void	rotate_around_x_five_degree(t_vec *vec)
{
	float	y_new;
	float	z_new;

	y_new = vec->y * cos(PI * 0.03) - (vec->z * sin(PI * 0.03));
	z_new = vec->y * sin(PI * 0.03) + (vec->z) * cos(PI * 0.03);
	vec->y = y_new;
	vec->z = z_new;
}

void	create_frame_test_zoom(t_vars *vars)
{
	int	i;
	t_vec *vec;

	i = 0;
	vec = vars->head->next;
	while (vec)
	{
		vars->x_values[i] *= 1.1;
		vars->y_values[i] *= 1.1;
		vars->z_values[i] *= 1.1;
		vec->x = vars->x_values[i];
		vec->y = vars->y_values[i];
		vec->z = vars->z_values[i];
		i++;
		vec = vec->next;
	}
	vec = vars->head;
	init_x_list(vec, vars);
	init_y_list(vec, vars);
	init_z_list(vec, vars);
	lstiter(vec, &rotate, vars);
	lstiter_rx(vec, &rotate_around_x);
}



void	increase_vars(t_vars *vars, int *x, int *i, int *k)
{
	*x = *x + vars->scale;
	(*i)++;
	(*k)++;
}

void	create_frame_zoom(t_vars *vars, int io)
{
	t_vec	*tmp;
	// int		i;

	tmp = vars->head->next;
	// i = 0;
	while (tmp)
	{
		if (io == 1)
		{
			tmp->x = tmp->x * 1.1;
			tmp->y = tmp->y * 1.1;
 			tmp = tmp->next;
		}
		else if (io == 0)
		{
			tmp->x = tmp->x / 1.1;
			tmp->y = tmp->y / 1.1;
			tmp = tmp->next;
		}
	}
	// tmp = vars->head->next;
	// while (tmp)
	// {
	// 	vars->x_values[i] = tmp->x;
	// 	vars->y_values[i] = tmp->y;
	// 	vars->y_values[i] = tmp->z;
	// 	i++;
	// 	tmp = tmp->next;
	// }
}

void	draw_to_next_row(t_vec *vec, t_data *img, int length)
{
	t_vec	*next_vec;
	int		i;

	next_vec = vec;
	i = 0;
	while (i < length)
	{
		next_vec = next_vec->next;
		if (next_vec == NULL)
			return ;
		i++;
	}
	bresenham(img, vec, next_vec);
}

int	open_file(int argc, char **argv)
{
	int	fd;

	fd = open(argv[argc - 1], O_RDONLY);
	if (fd == -1)
		exit(1);
	return (fd);
}

t_vec *create_frame(t_vars *vars, int argc, char **argv)
{
	t_vec	*vec;
	int		fd;
	int		x;
	int		y;

	fd = open_file(argc, argv);
	x = (1920 / 2) - vars->l / 2;
	y = (1080 / 2) - vars->w / 2;
	vec = create_frame_new(fd, vars, x, y);
	init_x_list(vec, vars);
	init_y_list(vec, vars);
	init_z_list(vec, vars);
	lstiter(vec, &rotate, vars);
	lstiter_rx(vec, &rotate_around_x);
	init_x_list_dyn(vec, vars);
	init_y_list_dyn(vec, vars);
	init_z_list_dyn(vec, vars);
	return (vec);
}

// void	create_frame_increase_z(t_vars *vars)
// {
	// // t_vec	*vec;
	// // t_vec	*tmp;
	// int		i = 0;
	// int		fd;
	// int		x;
	// int		y;

	// fd = open_file(vars->argc, vars->argv);
	// x = (1920 / 2) - vars->l / 2;
	// y = (1080 / 2) - vars->w / 2;
	// // tmp = vars->head->next;
	// // while (tmp)
	// // {
	// // 	if (tmp->z > 0)
	// // 		tmp->z = tmp->z + 10;
	// // 	tmp = tmp->next;
	// // }
	// while (i < vars->i_z)
	// {
	// 	if (vars->z_values[i] > 0)
	// 		vars->z_values[i] = vars->z_values[i] + 15;
	// 	i++;
	// }
	// create_frame_test(vars);
	// init_x_list(vec, vars);
	// init_y_list(vec, vars);
	// init_z_list(vec, vars);
	// lstiter(vec, &rotate, vars);
	// lstiter_rx(vec, &rotate_around_x);
// }

void	draw_map(t_vars *vars, int argc, char **argv)
{
	t_vec	*vec;
	t_vec	*next_vec;
	int 	i;
	int		eor;

	vec = create_frame(vars, argc, argv);
	vars->head = vec;
	vec = vec->next;
	next_vec = vec->next;
	i = 0;
	while (next_vec)
	{
		bresenham(&vars->img, vec, next_vec);
		manage_row(&vec, &next_vec);
		i++;
		eor = i % (vars->length - 1);
		if (!eor && next_vec != NULL)
			manage_row(&vec, &next_vec);
	}
	manage_nodes(vars->head, &vec, &next_vec, 'n');
	while (next_vec)
	{
		draw_to_next_row(vec, &vars->img, vars->length);
		manage_row(&vec, &next_vec);
	}
}

void	draw_zoom_map(t_vec *head, t_vars *vars, char mode)
{
	t_vec	*vec;
	t_vec	*next_vec;
	int		i;
	int		eor;

	vec = head->next;
	next_vec = vec->next;
	i = 0;
	while (next_vec)
	{
		bresenham(&vars->img, vec, next_vec);
		get_next_nodes(&vec, &next_vec);
		i++;
		eor = i % (vars->length - 1);
		if (!eor && next_vec != NULL)
			get_next_nodes(&vec, &next_vec);
 	}
	manage_nodes(head, &vec, &next_vec, mode);
	while (next_vec)
	{
		draw_to_next_row(vec, &vars->img, vars->length);
		manage_row(&vec, &next_vec);
	}
}

void	manage_direction(t_vec **vec, char mode)
{
	if (mode == 'd')
		(*vec)->y = (*vec)->y + 10;
	else if (mode == 'u')
		(*vec)->y = (*vec)->y - 10;
	else if (mode == 'r')
		(*vec)->x = (*vec)->x + 10;
	else if (mode == 'l')
		(*vec)->x = (*vec)->x - 10;
	*vec = (*vec)->next;
}

void	move_map_new(t_vec *head, t_vars *vars, char mode)
{
	t_vec	*vec;
	t_vec	*next_vec;
	int		i;
	int		eor;

	vec = head;
	i = 0;
	while (vec)
		manage_direction(&vec, mode);
	reset_nodes(&vec, &next_vec, vars); // is this valid?;
	while (next_vec)
	{
		bresenham(&vars->img, vec, next_vec);
		get_next_nodes(&vec, &next_vec);
		i++;
		eor = i % (vars->length - 1);
		if (!eor && next_vec != NULL)
			get_next_nodes(&vec, &next_vec);
	}
	reset_nodes(&vec, &next_vec, vars);
	while (next_vec)
	{
		draw_to_next_row(vec, &vars->img, vars->length);
		manage_row(&vec, &next_vec);
	}
}

void	zoom_map(t_vars *vars, int io)
{
	create_frame_zoom(vars, io);
	draw_zoom_map(vars->head, vars, '0');
}

// void	try_out_zoom(t_vars *vars, int io)
// {
// 	int	i;

// 	i = 0;
// 	io = 1;
// 	while (i < vars->i_x)
// 	{
// 		vars->x_values[i] = vars->x_values[i] * 1.1;
// 		i++;
// 	}
// 	i = 0;
// 	while (i < vars->i_y)
// 	{
// 		vars->y_values[i] = vars->y_values[i] * 1.1;
// 		i++;
// 	}
// }

void	init_values(t_vars *vars)
{
	t_vec	*vec;
	int	i;

	i = 0;
	vec = vars->head->next;
	while (vec)
	{
		vec->x = vars->x_values[i];
		vec->y = vars->y_values[i];
		vec->z = vars->z_values[i];
		i++;
		vec = vec->next;
	}
}

// t_vec *create_frame_ultimate_zoom(t_vars *vars)
// {
// 	t_vec	*vec;
// 	// t_vec	*tmp;
// 	// int		i = 0;
// 	int		fd;
// 	int		x;
// 	int		y;

// 	fd = open_file(vars->argc, vars->argv);
// 	x = (1920 / 2) - vars->l / 2;
// 	y = (1080 / 2) - vars->w / 2;
// 	// tmp = vars->head->next;
// 	// while (tmp)
// 	// {
// 	// 	if (tmp->z > 0)
// 	// 		tmp->z = tmp->z + 10;
// 	// 	tmp = tmp->next;
// 	// }
// 	// while (i < vars->i_z)
// 	// {
// 	// 	if (vars->z_values[i] > 0)
// 	// 		vars->z_values[i] = vars->z_values[i] + 15;
// 	// 	i++;
// 	// }
// 	// vec = create_frame_test(fd, vars, x, y);
// 	init_x_list(vec, vars);
// 	init_y_list(vec, vars);
// 	init_z_list(vec, vars);
// 	lstiter(vec, &rotate, vars);
// 	lstiter_rx(vec, &rotate_around_x);
// 	return (vec);
// }

int	key_hook(int keycode, t_vars *vars)
{
	t_vec *vec;
	t_vec *next_vec;
	int	i;
	int	eor;
	float before;
	float after;
	float before_y;
	float after_y;
	mlx_destroy_image(vars->mlx, vars->img.img);
	// printf("%d\n", keycode);
	// vars->img.img = mlx_new_image(vars->mlx, (vars->length * 3 * vars->scale), (vars->width * 3 * vars->scale));
	vars->img.img = mlx_new_image(vars->mlx, 1920, 1080);
	if (keycode == 13)
	{
		vec = vars->head->next;
		before = vars->head->next->y;
		lstiter_rx(vec, &rotate_around_x_five_degree);
		after = vars->head->next->y;
		vec = vars->head->next;
		while (vec)
		{
			vec->y = vec->y + (before - after);
			vec = vec->next;
		}
		vec = vars->head->next;
		next_vec = vec->next; // node 2;
		i = 0;
		while (next_vec)
		{
			bresenham(&vars->img, vec, next_vec);
			manage_row(&vec, &next_vec);
			i++;
			eor = i % (vars->length - 1);
			if (!eor && next_vec != NULL)
				manage_row(&vec, &next_vec);
		}
		manage_nodes(vars->head, &vec, &next_vec, 'n');
		while (next_vec)
		{
			draw_to_next_row(vec, &vars->img, vars->length);
			manage_row(&vec, &next_vec);
		}
		vars->w = vars->w + (before - after);
	}
	if (keycode == 53)
		exit (0);
	if (keycode == 123)
	{
		// vec = create_frame_increase_z(vars);
		draw_zoom_map(vec, vars, '0');
		// move_map_new(vars->head, vars, 'l');
	}
	if (keycode == 124)
		move_map_new(vars->head, vars, 'r');
	if (keycode == 125)
		move_map_new(vars->head, vars, 'd');
	if (keycode == 126) // pfeiltaste oben = rotate around x;
	{
		vec = vars->head->next;
		before = vars->head->next->y;
		lstiter_rx(vec, &rotate_around_x_five_degree);
		after = vars->head->next->y;
		init_x_list(vars->head, vars);
		init_y_list(vars->head, vars);
		init_z_list(vars->head, vars);
		vec = vars->head->next;
		while (vec)
		{
			vec->y = vec->y + (before - after);
			vec = vec->next;
		}
		i = 0;
		vec = vars->head->next;
		next_vec = vec->next;
		while (next_vec)
		{
			bresenham(&vars->img, vec, next_vec);
			manage_row(&vec, &next_vec);
			i++;
			eor = i % (vars->length - 1);
			if (!eor && next_vec != NULL)
				manage_row(&vec, &next_vec);
		}
		manage_nodes(vars->head, &vec, &next_vec, 'n');
		while (next_vec)
		{
			draw_to_next_row(vec, &vars->img, vars->length);
			manage_row(&vec, &next_vec);
		}
		vars->rota_counter++;
	}
	if (keycode == 24) // zoom in;
	{
		before = vars->head->next->x;
		before_y = vars->head->next->y;
		// // zoom_map(vars, 1);
		create_frame_zoom(vars, 1);
		after = vars->head->next->x;
		after_y = vars->head->next->y;
		vec = vars->head->next;
		while (vec)
		{
			vec->x = vec->x - (after - before);
			vec->y = vec->y - (after_y - before_y);
			vec = vec->next;
		}
		draw_zoom_map(vars->head, vars, '0');
		// vars->scale++;
		// draw_map(vars, vars->argc, vars->argv);
		// vec = create_frame_ultimate_zoom(vars);
		// create_frame_test_zoom(vars);
		// draw_zoom_map(vars->head, vars, '0');
	}
	if (keycode == 27) // zoom out;
	{
		before = vars->head->next->x;
		before_y = vars->head->next->y;
		create_frame_zoom(vars, 0);
		after = vars->head->next->x;
		after_y = vars->head->next->y;
		vec = vars->head->next;
		while (vec)
		{
			vec->x = vec->x + (before - after);
			vec->y = vec->y + (before_y - after_y);
			vec = vec->next;
		}
		draw_zoom_map(vars->head, vars, '0');
		// vars->scale--;
		// draw_map(vars, vars->argc, vars->argv);
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}

int	red_cross(void)
{
	exit(0);
	return (0);
}

int	get_width_and_length(int argc, char **argv, t_vars *vars)
{
	int		fd;
	int		width;
	int		length;
	char	*line;
	char	**split;

	fd = open_file(argc, argv);
	width = 0;
	length = 0;
	line = get_next_line(fd);
	if (line != NULL)
	{
		split = ft_split(line, ' ');
		while (split[length])
			length++;
	}
	vars->length = length;
	while (line)
	{
		width++;
		line = get_next_line(fd);
	}
	close(fd);
	return (width);
}

void	adjust_scale(t_vars *vars, int *length, int *width)
{
	if (*length >= 6000)
		vars->scale = vars->scale / 4;
	// *length = (vars->length * 3 * vars->scale);
	// *width = (vars->width * 3 * vars->scale);
	length = 0;
	width = 0;
}

int	main(int argc, char **argv)
{
	// int		fd;
	// int		length;
	// int		width;
	t_vars	vars;

	vars.width = get_width_and_length(argc, argv, &vars);
	// fd = open_file(argc, argv);
	// if (fd == -1)
	// 	return (1);
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 1920, 1080, "FdF");
	vars.scale = 20;
	// length = (vars.length * 3 * vars.scale);
	// width = (vars.width * 3 * vars.scale);
	vars.l = vars.length * vars.scale;
	vars.w = vars.width * vars.scale;
	vars.argc = argc;
	vars.argv = argv;
	vars.i_x = 0;
	vars.i_y = 0;
	vars.i_z = 0;
	vars.rota_counter = 0;
	// printf("l: %d\n", length);
	// printf("w: %d\n", width);
	// adjust_scale(&vars, &length, &width);
	// vars.img.img = mlx_new_image(vars.mlx, length, width);
	vars.img.img = mlx_new_image(vars.mlx, 1920, 1080);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length,
									&vars.img.endian);
	// vars.l = 1920/2 - (vars.length * 3 * vars.scale) / 2;
	// vars.w = 1080/2 - (vars.width * 3 * vars.scale) / 2;
	draw_map(&vars, argc, argv);
	// mlx_put_image_to_window(vars.mlx, vars.win, vars.img.img, 1920/2 - (length / 2), 1080/2 - (width / 2));
	mlx_put_image_to_window(vars.mlx, vars.win, vars.img.img, 0, 0);
	mlx_key_hook(vars.win, key_hook, &vars);
	mlx_hook(vars.win, 17, 0L, red_cross, &vars);
	mlx_loop(vars.mlx);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:48:24 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/09/21 16:42:10 by fyuzhyk          ###   ########.fr       */
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
	x = (int)round(vec->x);
	y = (int)round(vec->y);
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
	x = (int)round(vec->x);
	y = (int)round(vec->y);
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

void init_pos_list(t_vec *vec, t_vars *vars)
{
	int		i;
	t_vec	*tmp;

	i = 0;
	tmp = vec->next;
	while (tmp)
	{
		vars->x_values[i] = tmp->x;
		vars->y_values[i] = tmp->y;
		vars->z_values[i] = tmp->z;
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
	double	x_new;
	double	y_new;

	x_new = vec->x - x * cos(PI/4) - vec->y - y * sin(PI/4);
	y_new = vec->x - x * sin(PI/4) + vec->y - y * cos(PI/4);
	vec->x = x_new + x;
	vec->y = y_new + y;
}

void	rotate_around_x(t_vec *vec)
{
	double	y_new;
	double	z_new;

	y_new = (vec->y * cos(PI/4)) - (vec->z * sin(PI/4));
	z_new = (vec->y * sin(PI/4) + (vec->z) * cos(PI/4));
	vec->y = y_new;
	vec->z = z_new;
}

void	rotate_around_x_custom_angle(t_vec *vec, double angle)
{
	double	y_new;
	double	z_new;

	y_new = vec->y * cos(PI * angle) - (vec->z * sin(PI * angle));
	z_new = vec->y * sin(PI * angle) + (vec->z) * cos(PI * angle);
	vec->y = y_new;
	vec->z = z_new;
}

void	rotate_around_x_custom_angle_backwards(t_vec *vec, double angle)
{
	double	y_new;
	double	z_new;

	y_new = vec->y * cos(PI * angle) + vec->z * sin(PI * angle);
	z_new = vec->y * -(sin(PI * angle)) + vec->z * cos(PI * angle);
	vec->y = y_new;
	vec->z = z_new;
}

void	rotate_around_y_custom_angle(t_vec *vec, double angle)
{
	double	x_new;
	double	z_new;

	x_new = vec->x * cos(PI * angle) - (vec->z * sin(PI * angle));
	z_new = vec->x * sin(PI * angle) + (vec->z) * cos(PI * angle);
	vec->x = x_new;
	vec->z = z_new;
}

void	rotate_around_y_custom_angle_backwards(t_vec *vec, double angle)
{
	double	x_new;
	double	z_new;

	x_new = vec->x * cos(PI * angle) + vec->z * sin(PI * angle);
	z_new = vec->x * -(sin(PI * angle)) + vec->z * cos(PI * angle);
	vec->x = x_new;
	vec->z = z_new;
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

	tmp = vars->head->next;
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
	init_pos_list(vec, vars);
	lstiter(vec, &rotate, vars);
	lstiter_rx(vec, &rotate_around_x);
	return (vec);
}

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
	manage_nodes(vars->head, &vec, &next_vec, '0');
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
	t_vec	*vec;
	double	before_x;
	double	before_y;
	double	after_x;
	double	after_y;

	before_x = vars->head->next->x;
	before_y = vars->head->next->y;
	create_frame_zoom(vars, io);
	after_x = vars->head->next->x;
	after_y = vars->head->next->y;
	vec = vars->head->next;
	while (vec)
	{
		vec->x = vec->x - (after_x - before_x);
		vec->y = vec->y - (after_y - before_y);
		vec = vec->next;
	}
	draw_zoom_map(vars->head, vars, '0');
}

void	simple_draw(t_vars *vars)
{
	int		i;
	int		eor;
	t_vec	*vec;
	t_vec	*next_vec;

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
}

void	rotate_operation_x(t_vars *vars)
{
	t_vec	*vec;
	double	before;
	double	after;

	vec = vars->head->next;
	before = vec->y;
	while (vec)
	{
		rotate_around_x_custom_angle(vec, vars->angle);
		vec = vec->next;
	}
	vec = vars->head->next;
	after = vec->y;
	while (vec)
	{
		vec->y = vec->y + (before - after);
		vec = vec->next;
	}
}

void	rotate_operation_x_backwards(t_vars *vars)
{
	t_vec	*vec;
	double	before;
	double	after;

	vec = vars->head->next;
	before = vec->y;
	while (vec)
	{
		rotate_around_x_custom_angle_backwards(vec, vars->angle);
		vec = vec->next;
	}
	vec = vars->head->next;
	after = vec->y;
	while (vec)
	{
		vec->y = vec->y - (after - before);
		vec = vec->next;
	}
}

void	rotate_operation_y(t_vars *vars)
{
	t_vec	*vec;
	double	before;
	double	after;

	vec = vars->head->next;
	before = vec->x;
	while (vec)
	{
		rotate_around_y_custom_angle(vec, vars->angle);
		vec = vec->next;
	}
	vec = vars->head->next;
	after = vec->x;
	while (vec)
	{
		vec->x = vec->x + (before - after);
		vec = vec->next;
	}
}

void	rotate_operation_y_backwards(t_vars *vars)
{
	t_vec	*vec;
	double	before;
	double	after;

	vec = vars->head->next;
	before = vec->x;
	while (vec)
	{
		rotate_around_y_custom_angle_backwards(vec, vars->angle);
		vec = vec->next;
	}
	vec = vars->head->next;
	after = vec->x;
	while (vec)
	{
		vec->x = vec->x - (after - before);
		vec = vec->next;
	}
}


void	define_z_value(t_vars *vars, t_vec *vec, int id, int i)
{
	if (id == 0) // increase;
	{
		if (vars->z_values[i] > 0)
		{
			vars->z_values[i] += 10;
			vec->z = vars->z_values[i];
		}
		else
			vec->z = vars->z_values[i];
	}
	else // decrease;
	{
		if (vars->z_values[i] > 0)
		{
			vars->z_values[i] -= 10;
			vec->z = vars->z_values[i];
			if (vars->z_values[i] == 0)
				vars->z_values[i] += 10;
		}
		else
			vec->z = vars->z_values[i];
	}
}

void	set_initial_values(t_vars *vars, int id)
{
	int		i;
	t_vec	*vec;

	i = 0;
	vec = vars->head->next;
	while (vec)
	{
		vec->x = vars->x_values[i];
		vec->y = vars->y_values[i];
		define_z_value(vars, vec, id, i);
		i++;
		vec = vec->next;
	}
	vec = vars->head->next;
	lstiter(vec, &rotate, vars);
	lstiter_rx(vec, &rotate_around_x);
}

void	get_zoom_amount(t_vars *vars)
{
	int	zoom_amount;
}

void	create_frame_increase_z(t_vars *vars, int id)
{
	int			i;
	int			zoom_amount;
	double		before;
	double		before_y;
	double		after;
	double		after_y;
	double		angle;
	t_vec		*vec;

	i = 0;
	set_initial_values(vars, id);
	zoom_amount = vars->zoom_counter - vars->neg_zoom_counter;
	if (zoom_amount > 0)
	{
		i = 0;
		before = vars->head->next->x;
		before_y = vars->head->next->y;
		while (i < zoom_amount)
		{
			create_frame_zoom(vars, 1);
			i++;
		}

		after = vars->head->next->x;
		after_y = vars->head->next->y;
		vec = vars->head->next;
		while (vec)
		{
			vec->x = vec->x - (after - before);
			vec->y = vec->y - (after_y - before_y);
			vec = vec->next;
		}
	}
	else
	{
		zoom_amount = -zoom_amount;
		i = 0;
		before = vars->head->next->x;
		before_y = vars->head->next->y;
		while (i < zoom_amount)
		{
			create_frame_zoom(vars, 0);
			i++;
		}
		after = vars->head->next->x;
		after_y = vars->head->next->y;
		vec = vars->head->next;
		while (vec)
		{
			vec->x = vec->x + (before - after);
			vec->y = vec->y + (before_y - after_y);
			vec = vec->next;
		}
	}
	if (vars->rota_counter > 0)
	{
		angle = vars->rota_counter * vars->angle;
		vec = vars->head->next;
		before = vec->y;
		while (vec)
		{
			rotate_around_x_custom_angle(vec, angle);
			vec = vec->next;
		}
		vec = vars->head->next;
		after = vec->y;
		while (vec)
		{
			vec->y = vec->y + (before - after);
			vec = vec->next;
		}
	}
}

void	create_frame_decrease_z(t_vars *vars)
{
	int			i;
	int			zoom_amount;
	double		before;
	double		before_y;
	double		after;
	double		after_y;
	double		angle;
	t_vec		*vec;

	i = 0;
	vec = vars->head->next;
	while (vec) // init the raw values here;
	{
		vec->x = vars->x_values[i];
		vec->y = vars->y_values[i];
		if (vars->z_values[i] > 0)
		{
			vars->z_values[i] -= 10;
			vec->z = vars->z_values[i];
			if (vars->z_values[i] == 0)
				vars->z_values[i] += 10;
		}
		else
			vec->z = vars->z_values[i];
		i++;
		vec = vec->next;
	}
	vec = vars->head->next;
	lstiter(vec, &rotate, vars);
	lstiter_rx(vec, &rotate_around_x);
	zoom_amount = vars->zoom_counter - vars->neg_zoom_counter;
	if (zoom_amount > 0)
	{
		i = 0;
		before = vars->head->next->x;
		before_y = vars->head->next->y;
		while (i < zoom_amount)
		{
			create_frame_zoom(vars, 1);
			i++;
		}

		after = vars->head->next->x;
		after_y = vars->head->next->y;
		vec = vars->head->next;
		while (vec)
		{
			vec->x = vec->x - (after - before);
			vec->y = vec->y - (after_y - before_y);
			vec = vec->next;
		}
	}
	else
	{
		zoom_amount = -zoom_amount;
		i = 0;
		before = vars->head->next->x;
		before_y = vars->head->next->y;
		while (i < zoom_amount)
		{
			create_frame_zoom(vars, 0);
			i++;
		}
		after = vars->head->next->x;
		after_y = vars->head->next->y;
		vec = vars->head->next;
		while (vec)
		{
			vec->x = vec->x + (before - after);
			vec->y = vec->y + (before_y - after_y);
			vec = vec->next;
		}
	}
	if (vars->rota_counter > 0)
	{
		angle = vars->rota_counter * vars->angle;
		vec = vars->head->next;
		before = vec->y;
		while (vec)
		{
			rotate_around_x_custom_angle(vec, angle);
			vec = vec->next;
		}
		vec = vars->head->next;
		after = vec->y;
		while (vec)
		{
			vec->y = vec->y + (before - after);
			vec = vec->next;
		}
	}
}

int	key_hook(int keycode, t_vars *vars)
{
	mlx_destroy_image(vars->mlx, vars->img.img);
	// printf("%d\n", keycode);
	// vars->img.img = mlx_new_image(vars->mlx, (vars->length * 3 * vars->scale), (vars->width * 3 * vars->scale));
	vars->img.img = mlx_new_image(vars->mlx, 1920, 1080);
	if (keycode == 13) // w;
	{
		rotate_operation_x(vars);
		simple_draw(vars);
		vars->rota_counter++;
	}
	if (keycode == 1) // s;
	{
		rotate_operation_x_backwards(vars);
		simple_draw(vars);
	}
	if (keycode == 0) // a;
	{
		rotate_operation_y(vars);
		simple_draw(vars);
	}
	if (keycode == 2) // d;
	{
		rotate_operation_y_backwards(vars);
		simple_draw(vars);
	}
	if (keycode == 53)
		exit (0);
	if (keycode == 123) // linke pfeiltaste => increase z;
	{
		create_frame_increase_z(vars, 0);
		simple_draw(vars);
		// move_map_new(vars->head, vars, 'l');
	}
	if (keycode == 124) // rechte pfeiltaste => decrease z;
	{
		create_frame_decrease_z(vars);
		simple_draw(vars);
		// move_map_new(vars->head, vars, 'r');
	}
	if (keycode == 125)
		move_map_new(vars->head, vars, 'd');
	if (keycode == 126)
		move_map_new(vars->head, vars, 'u');
	if (keycode == 24) // zoom in;
	{
		zoom_map(vars, 1);
		vars->zoom_counter++;
	}
	if (keycode == 27) // zoom out;
	{
		zoom_map(vars, 0);
		vars->neg_zoom_counter++;
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
	// int		length;
	// int		width;
	t_vars	vars;

	vars.width = get_width_and_length(argc, argv, &vars);
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 1920, 1080, "FdF");
	vars.scale = 20;
	// length = (vars.length * 3 * vars.scale);
	// width = (vars.width * 3 * vars.scale);
	vars.l = vars.length * vars.scale;
	vars.w = vars.width * vars.scale;
	vars.argc = argc;
	vars.argv = argv;
	vars.i_z = 0;
	vars.rota_counter = 0;
	vars.zoom_counter = 0;
	vars.neg_zoom_counter = 0;
	vars.angle = 0.028;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:48:24 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/09/16 15:36:18 by fyuzhyk          ###   ########.fr       */
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
	t_data	img;
	t_vec 	*head;
	void	*mlx;
	void	*win;
	int		scale;
	int		length;
	int		width;
	int		fd;
	int		z_values[5000];
	int		y_values[5000];
	int		x_values[5000];
}				t_vars;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	manage_direction(t_vec **vec, t_vec **next_vec, char mode)
{
		if (mode == 'd')
			(*vec)->y++;
		else if (mode == 'u')
			(*vec)->y--;
		else if (mode == 'r')
			(*vec)->x++;
		else if (mode == 'l')
			(*vec)->x--;
		*vec = (*vec)->next;
		*next_vec = (*vec)->next;
}

void	manage_nodes(t_vec *head, t_vec **vec, t_vec **next_vec, char mode)
{
		if (mode == 'd')
			(*vec)->y++;
		else if (mode == 'u')
			(*vec)->y--;
		else if (mode == 'r')
			(*vec)->x++;
		else if (mode == 'l')
			(*vec)->x--;
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

	dx = next_vec->x - vec->x;
	return (dx);
}

int	get_dy(t_vec *vec, t_vec *next_vec)
{
	int	dy;

	dy = next_vec->y - vec->y;
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
	x = vec->x;
	y = vec->y;
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
	x = vec->x;
	y = vec->y;
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

void	bresen_test(t_data *img, t_vec *vec, t_vec *next_vec)
{
	int	dx;
	int dy;

	dx = next_vec->x - vec->x;
	dy = next_vec->y - vec->y;
	if (get_absdx(dx) > get_absdy(dy))
		bresen_slope_more_than_one(vec, next_vec, img);
	else
		bresen_slope_less_than_one(vec, next_vec, img);
}

int	get_z(t_vars *vars, char **split)
{
	int	z;

	z = ft_atoi(split[vars->length]);
	if (z > 0)
		z += vars->scale;
	else if (z < 0)
		z -= vars->scale;
	return (z);
}

t_vec	*create_frame_new(int fd, t_vars *vars, int x, int y)
{
	char	*line;
	char	**ps;
	t_vec	*vec;

	vec = new_vec(0, 0, 0);
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
		vars->width++;
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

int	get_z_zoom(t_vars *vars, int io, int *k)
{
	if (vars->z_values[*k] > 0)
	{
		if (io == 1)
			vars->z_values[*k]++;
		else
			vars->z_values[*k]--;
	}
	else if (vars->z_values[*k] < 0)
	{
		if (io == 1)
			vars->z_values[*k]--;
		else
			vars->z_values[*k]++;
	}
	return (vars->z_values[*k]);
}

int	get_x_zoom(t_vars *vars, int io, int *k)
{
	if (vars->x_values[*k] > 0)
	{
		if (io == 1)
			vars->x_values[*k]++;
		else
			vars->x_values[*k]--;
	}
	else if (vars->x_values[*k] < 0)
	{
		if (io == 1)
			vars->x_values[*k]--;
		else
			vars->x_values[*k]++;
	}
	return (vars->x_values[*k]);
}

int	get_y_zoom(t_vars *vars, int io, int *k)
{
	if (vars->y_values[*k] > 0)
	{
		if (io == 1)
			vars->y_values[*k]++;
		else
			vars->y_values[*k]--;
	}
	else if (vars->y_values[*k] < 0)
	{
		if (io == 1)
			vars->y_values[*k]--;
		else
			vars->y_values[*k]++;
	}
	return (vars->y_values[*k]);
}

void	increase_vars(t_vars *vars, int *x, int *i, int *k)
{
	*x = *x + vars->scale;
	(*i)++;
	(*k)++;
}

t_vec	*create_frame_zoom(t_vars *vars, int io, int x, int y)
{
	int		i;
	int		j;
	int		k;
	t_vec	*vec;

	j = 0;
	k = 0;
	vec = new_vec(0, 0, 0);
	while (j < vars->width)
	{
		i = 0;
		while (i < vars->length)
		{
			lstadd_back(&vec, new_vec(x, y, get_z_zoom(vars, io, &k)));
			increase_vars(vars, &x, &i, &k);
		}
		x -= (i * vars->scale); // set x back to the initial value;
		y += vars->scale;
		j++;
	}
	return (vec);
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
	bresen_test(img, vec, next_vec);
}

void	init_z_array(t_vars *vars, t_vec *vec)
{
	int	i;

	i = 0;
	while (vec)
	{
		vars->z_values[i] = vec->z;
		i++;
		vec = vec->next;
	}
}

void	init_x_array(t_vars *vars, t_vec *vec)
{
	int	i;

	i = 0;
	while (vec)
	{
		vars->x_values[i] = vec->x;
		i++;
		vec = vec->next;
	}
}

void	init_y_array(t_vars *vars, t_vec *vec)
{
	int	i;

	i = 0;
	while (vec)
	{
		vars->y_values[i] = vec->y;
		i++;
		vec = vec->next;
	}
}

t_vec *create_frame(int fd, t_vars *vars)
{
	t_vec	*vec;
	int		x;
	int		y;

	x = 0;
	y = 0;
	vec = create_frame_new(fd, vars, x, y);
	init_z_array(vars, vec);
	init_x_array(vars, vec);
	init_y_array(vars, vec);
	lstiter(vec, &rotate);
	lstiter_rx(vec, &rotate_around_x);
	return (vec);
}

void	draw_map(t_vars *vars, int fd)
{
	t_vec	*vec;
	t_vec	*next_vec;
	int 	i;
	int		eor;

	vec = create_frame(fd, vars);
	vars->head = vec;
	vec = vec->next;
	next_vec = vec->next;
	i = 0;
	while (next_vec)
	{
		bresen_test(&vars->img, vec, next_vec);
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

void	move_map(t_vec *head, t_vars *vars, char mode)
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
		bresen_test(&vars->img, vec, next_vec);
		manage_direction(&vec, &next_vec, mode);
		i++;
		eor = i % (vars->length - 1);
		if (!eor && next_vec != NULL)
			manage_direction(&vec, &next_vec, mode);
 	}
	manage_nodes(head, &vec, &next_vec, mode);
	while (next_vec)
	{
		draw_to_next_row(vec, &vars->img, vars->length);
		manage_row(&vec, &next_vec);
	}
	init_x_array(vars, vars->head);
	init_y_array(vars, vars->head);
}

int	key_hook(int keycode, t_vars *vars)
{
	t_vec *head;
	int	x;
	int	y;

	x = 0;
	y = 0;
	mlx_destroy_image(vars->mlx, vars->img.img);
	vars->img.img = mlx_new_image(vars->mlx, 6000, 6000);
	if (keycode == 53)
		exit (0);
	if (keycode == 123)
		move_map(vars->head, vars, 'l');
	if (keycode == 124)
		move_map(vars->head, vars, 'r');
	if (keycode == 125)
		move_map(vars->head, vars, 'd');
	if (keycode == 126)
		move_map(vars->head, vars, 'u');
	if (keycode == 24)
	{
		vars->scale++;
		head = create_frame_zoom(vars, 1, x, y);
		lstiter(head, &rotate);
		lstiter_rx(head, &rotate_around_x);
		move_map(head, vars, '0');
	}
	if (keycode == 27)
	{
		vars->scale--;
		head = create_frame_zoom(vars, 0, x, y);
		lstiter(head, &rotate);
		lstiter_rx(head, &rotate_around_x);
		move_map(head, vars, '0');
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}

// int mouse_hook(int mousecode, t_vars *vars)
// {
// 	t_vec *head;
// 	int	x;
// 	int	y;

// 	x = 0;
// 	y = 0;
// 	// mlx_destroy_image(vars->mlx, vars->img.img);
// 	// vars->img.img = mlx_new_image(vars->mlx, 6000, 6000);
// 	if (mousecode == 1)
// 	{
// 		printf("hehe\n");
// 		head = create_frame_zoom(vars, 1, x, y);
// 		lstiter(head, &rotate);
// 		lstiter_rx(head, &rotate_around_x);
// 		move_map(head, vars, '0');
// 	}
// 	else if (mousecode == 2)
// 	{
// 		printf("huhu\n");
// 		head = create_frame_zoom(vars, 0, x, y);
// 		lstiter(head, &rotate);
// 		lstiter_rx(head, &rotate_around_x);
// 		move_map(head, vars, '0');
// 	}
// 	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
// 	return (0);
// }

int	red_cross(void)
{
	exit(0);
	return (0);
}

int	main(int argc, char **argv)
{
	int		fd = open(argv[argc - 1], O_RDONLY);
	void	*mlx;
	void	*win;
	t_vars vars;

	mlx = mlx_init();
	vars.mlx = mlx;
	win = mlx_new_window(mlx, 1920, 1080, "FdF");
	vars.win = win;
	vars.img.img = mlx_new_image(mlx, 6000, 6000);
	vars.scale = 30;
	vars.fd = fd;
	vars.width = 0;

	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.line_length,
								&vars.img.endian);
	draw_map(&vars, fd);
	mlx_put_image_to_window(mlx, win, vars.img.img, 0, 0);
	mlx_key_hook(vars.win, key_hook, &vars);
	mlx_hook(vars.win, 17, 0L, red_cross, &vars);
	mlx_loop(mlx);
}

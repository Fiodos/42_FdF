/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:48:24 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/07 20:00:09 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/fdf.h"
#include "../libft/libft.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

// 810202 ==> 8454658 ==> 138757720
uint32_t	hex_to_hex(uint32_t hex)
{
	uint32_t	result;
	int			num;
	int			exponent;

	exponent = 0;
	while (hex)
	{
		num = hex % 10;
		result = result + (num * pow(16, exponent));
		hex = hex / 10;
		exponent++;
	}
	return (result);
}

uint32_t	ft_atoi_base(const char *color)
{
	uint32_t	converted_color;
	int			index;
	int			len;
	int			i;

	char *base16 = "0123456789abcdef";
	converted_color = 0;
	len = ft_strlen(color) - 1;
	i = 0;
	while (color[i])
	{
		index = 0;
		while (color[i] != base16[index])
			index++;
		converted_color = converted_color + (index * pow(16, len));
		len--;
		i++;
	}
	converted_color = hex_to_hex(converted_color);
	return (converted_color);
}


int	ft_atoi_fdf(const char *str, t_vars_new *vars)
{
	int	i;
	int	minus_count;
	int	num;

	i = 0;
	minus_count = 1;
	num = 0;
	while (str[i] == '\r' || str[i] == '\t' || str[i] == ' '
		|| str[i] == '\f' || str[i] == '\v' || str[i] == '\n')
		i++;
	if (str[i] == '-')
	{
		minus_count *= (-1);
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] <= '9' && str[i] >= '0')
	{
		num = (str[i] - '0') + (num * 10);
		i++;
	}
	if (str[i] == ',')
		vars->color = ft_atoi_base(&str[i + 3]);
	return (num * minus_count);
}

void	get_next_nodes(t_vec **vec, t_vec **next_vec)
{
	*vec = (*vec)->next;
	*next_vec = (*vec)->next;
}

void	reset_nodes(t_vec **vec, t_vec **next_vec, t_vars_new *vars)
{
	*vec = vars->head->next;
	*next_vec = (*vec)->next;
}

void	manage_nodes(t_vec *head, t_vec **vec, t_vec **next_vec)
{
	*vec = head->next;
	*next_vec = (*vec)->next;
}

void	manage_row(t_vec **vec, t_vec **next_vec)
{
	*vec = (*vec)->next;
	*next_vec = (*vec)->next;
}

int	check_range(int x, int y)
{
	if (x > 0 && x < 1920 && y > 0 && y < 1080)
		return (1);
	return (0);
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

void	bresen_slope_more_than_one(t_vec *vec, t_vec *next_vec, mlx_image_t *img)
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
		if (check_range(x, y))
			mlx_put_pixel(img, x, y, 0xFFFFFF);
			// mlx_put_pixel(img, x, y, vec->color);
		i++;
	}
}

void	bresen_slope_less_than_one(t_vec *vec, t_vec *next_vec, mlx_image_t *img)
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
		if (check_range(x, y))
			mlx_put_pixel(img, x, y, 0xFFFFFF);
			// mlx_put_pixel(img, x, y, vec->color);
		i++;
	}
}

void	bresenham(mlx_image_t *img, t_vec *vec, t_vec *next_vec)
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

int	get_z(t_vars_new *vars, char **split)
{
	int	z;

	// the 30 was a 15 previously;
	z = ft_atoi_fdf(split[vars->length], vars);
	if (z <= 30 && z > 0)
		z *= vars->scale;
	else if (z >= -30 && z < 0)
		z *= vars->scale;
	else if (z > 30 || z < -30)
		z *= 2;
	vars->z_values[vars->i_z] = z;
	vars->i_z++;
	return (z);
}

void init_pos_list(t_vec *vec, t_vars_new *vars)
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

t_vec	*create_frame_new(int fd, t_vars_new *vars, int x, int y)
{
	char	*line;
	char	**ps;
	t_vec	*vec;
	t_vec	*next;
	t_vec	*tmp;

	vec = new_vec(x, y, 0);
	tmp = vec;
	line = get_next_line(fd);
	while (line)
	{
		vars->length = 0;
		ps = ft_split(line, ' ');
		while (ps[vars->length])
		{
			next = new_vec(x, y, get_z(vars, ps));
			lstadd_back(&tmp, &next, vars);
			x += vars->scale;
		}
		x = x - (vars->length * vars->scale);
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

	x_new = (vec->x - x) * cos(PI/4) - (vec->y - y) * sin(PI/4);
	y_new = (vec->x - x) * sin(PI/4) + (vec->y - y) * cos(PI/4);
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

void	rotate_around_y_custom_angle(t_vec *vec, double angle)
{
	double	x_new;
	double	z_new;

	x_new = vec->x * cos(PI * angle) - (vec->z * sin(PI * angle));
	z_new = vec->x * sin(PI * angle) + (vec->z) * cos(PI * angle);
	vec->x = x_new;
	vec->z = z_new;
}

void	draw_to_next_row(t_vec *vec, mlx_image_t *img, int length)
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

t_vec *create_frame(t_vars_new *vars, int argc, char **argv)
{
	t_vec	*vec;
	int		fd;
	int		x;
	int		y;

	fd = open_file(argc, argv);
	x = (1920 / 2);
	y = (1080 / 2);
	vec = create_frame_new(fd, vars, x, y);
	init_pos_list(vec, vars);
	lstiter(vec, &rotate);
	lstiter_rx(vec, &rotate_around_x);
	return (vec);
}

void	draw_map(t_vars_new *vars, int argc, char **argv)
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
		bresenham(vars->img, vec, next_vec);
		manage_row(&vec, &next_vec);
		i++;
		eor = i % (vars->length - 1);
		if (!eor && next_vec != NULL)
			manage_row(&vec, &next_vec);
	}
	manage_nodes(vars->head, &vec, &next_vec);
	while (next_vec)
	{
		draw_to_next_row(vec, vars->img, vars->length);
		manage_row(&vec, &next_vec);
	}
}

void	simple_draw(t_vars_new *vars)
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
		bresenham(vars->img, vec, next_vec);
		manage_row(&vec, &next_vec);
		i++;
		eor = i % (vars->length - 1);
		if (!eor && next_vec != NULL)
			manage_row(&vec, &next_vec);
	}
	manage_nodes(vars->head, &vec, &next_vec);
	while (next_vec)
	{
		draw_to_next_row(vec, vars->img, vars->length);
		manage_row(&vec, &next_vec);
	}
}

void	increase_z(t_vars_new *vars, t_vec *vec, int i)
{
	if (vars->z_values[i] > 0)
	{
		vars->z_values[i] *= 1.1;
		vec->z = vars->z_values[i];
	}
	else if (vars->z_values[i] < 0)
	{
		vars->z_values[i] *= 1.1;
		vec->z = vars->z_values[i];
	}
	else
		vec->z = vars->z_values[i];
}

void	decrease_z(t_vars_new *vars, t_vec *vec, int i)
{
	if (vars->z_values[i] > 0)
	{
		if (vars->z_values[i] / 1.1 == 0 || vars->z_values[i] / 1.1 < 0)
			vars->z_values[i] = vars->z_values[i];
		vars->z_values[i] /= 1.1;
		vec->z = vars->z_values[i];
	}
	else if (vars->z_values[i] < 0)
	{
		vars->z_values[i] /= 1.1;
		vec->z = vars->z_values[i];
	}
	else
		vec->z = vars->z_values[i];
}

void	define_z_value(t_vars_new *vars, t_vec *vec, int id, int i)
{
	if (id == 0) // increase;
		increase_z(vars, vec, i);
	else if (id == 1) // decrease;
		decrease_z(vars, vec, i);
	else
		vec->z = vars->z_values[i];
}

void	set_initial_values(t_vars_new *vars, int id)
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
	lstiter(vec, &rotate);
	lstiter_rx(vec, &rotate_around_x);
}

void	create_zoomed_frame(t_vars_new *vars)
{
	t_vec *vec;
	double	before_x;
	double	before_y;
	double	after_x;
	double	after_y;

	vec = vars->head->next;
	before_x = vars->head->next->x;
	before_y = vars->head->next->y;
	while (vec) {
		vec->x *= vars->zoom;
		vec->y *= vars->zoom;
		vec->z *= vars->zoom;
		vec = vec->next;
	}
	after_x = vars->head->next->x;
	after_y = vars->head->next->y;
	vec = vars->head->next;
	while (vec)
	{
		vec->x = vec->x - (after_x - before_x);
		vec->y = vec->y - (after_y - before_y);
		vec = vec->next;
	}
}

void	rotate_operation_y(t_vars_new *vars, t_vec *vec)
{
	double	before_x;
	double	after_x;

	before_x = vec->x;
	while (vec)
	{
		rotate_around_y_custom_angle(vec, vars->acc_angle_y);
		vec = vec->next;
	}
	vec = vars->head->next;
	after_x = vec->x;
	while (vec)
	{
		if (vars->acc_angle_y)
			vec->x = vec->x - (after_x - before_x);
		else
			vec->x = vec->x + (before_x - after_x);
		vec = vec->next;
	}
}

void	rotate_operation_x(t_vars_new *vars, t_vec *vec)
{
	double	before_y;
	double	after_y;

	before_y = vec->y;
	while (vec)
	{
		rotate_around_x_custom_angle(vec, vars->acc_angle);
		vec = vec->next;
	}
	vec = vars->head->next;
	after_y = vec->y;
	while (vec)
	{
		if (vars->acc_angle < 0)
			vec->y = vec->y - (after_y - before_y);
		else
			vec->y = vec->y + (before_y - after_y);
		vec = vec->next;
	}
}

void	move_operation(t_vars_new *vars, t_vec *vec)
{
	while (vec)
	{
		vec->x += vars->move_x;
		vec->y += vars->move_y;
		vec = vec->next;
	}
}

void	create_new_frame(t_vars_new *vars, int id) // id defines either increase(0), decrease(1) or nothing(2);
{
	t_vec *vec;

	set_initial_values(vars, id);
	if (vars->zoom > 1 || vars->zoom < 1)
		create_zoomed_frame(vars);
	vec = vars->head->next;
	rotate_operation_y(vars, vec);
	vec = vars->head->next;
	rotate_operation_x(vars, vec);
	vec = vars->head->next;
	move_operation(vars, vec);
}

int	get_width_and_length(int argc, char **argv, t_vars_new *vars)
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

void	adjust_scale(t_vars_new *vars, double *length, double *width)
{
	if (*length >= 600)
	{
		vars->scale = vars->scale / 4;
	}
	*length = vars->length * vars->scale;
	*width = vars->width * vars->scale;
}

void	zoom_hook(t_vars_new *vars)
{
	mlx_delete_image(vars->mlx, vars->img);
	vars->img = mlx_new_image(vars->mlx, 1920, 1920);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_EQUAL))
		vars->zoom *= 1.1;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_MINUS))
		vars->zoom /= 1.1;
	create_new_frame(vars, 2);
	simple_draw(vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
}

void	rotate_x_hook(t_vars_new *vars)
{
	mlx_delete_image(vars->mlx, vars->img);
	vars->img = mlx_new_image(vars->mlx, 1920, 1080);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_W))
		vars->acc_angle += vars->angle;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_S))
		vars->acc_angle -= vars->angle;
	create_new_frame(vars, 2);
	simple_draw(vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
}

void	rotate_y_hook(t_vars_new *vars)
{
	mlx_delete_image(vars->mlx, vars->img);
	vars->img = mlx_new_image(vars->mlx, 1920, 1080);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_A))
		vars->acc_angle_y += vars->angle;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_D))
		vars->acc_angle_y -= vars->angle;
	create_new_frame(vars, 2);
	simple_draw(vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
}

void	change_z_hook(t_vars_new *vars)
{
	mlx_delete_image(vars->mlx, vars->img);
	vars->img = mlx_new_image(vars->mlx, 1920, 1080);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_1))
		create_new_frame(vars, 0);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_2))
		create_new_frame(vars, 1);
	simple_draw(vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
}

void	move_hook_sideways(t_vars_new *vars)
{
	mlx_delete_image(vars->mlx, vars->img);
	vars->img = mlx_new_image(vars->mlx, 1920, 1080);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_LEFT))
		vars->move_x -= 10;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_RIGHT))
		vars->move_x += 10;
	create_new_frame(vars, 2);
	simple_draw(vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
}

void	move_hook_up_down(t_vars_new *vars)
{
	mlx_delete_image(vars->mlx, vars->img);
	vars->img = mlx_new_image(vars->mlx, 1920, 1080);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_UP))
		vars->move_y -= 10;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_DOWN))
		vars->move_y += 10;
	create_new_frame(vars, 2);
	simple_draw(vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
}

void	hook(void *param)
{
	t_vars_new	*vars;

	vars = (t_vars_new *)param;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(vars->mlx);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_EQUAL)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_MINUS))
		zoom_hook(vars);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_W)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_S))
		rotate_x_hook(vars);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_A)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_D))
		rotate_y_hook(vars);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_1)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_2))
		change_z_hook(vars);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_LEFT)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_RIGHT))
		move_hook_sideways(vars);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_UP)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_DOWN))
		move_hook_up_down(vars);
}

void	allocate_value_arrays(t_vars_new *vars, int node_count)
{
	vars->x_values = malloc(sizeof(double) * node_count);
	if (vars->x_values == NULL)
		exit(1);
	vars->y_values = malloc(sizeof(double) * node_count);
	if (vars->y_values == NULL)
		exit(1);
	vars->z_values = malloc(sizeof(double) * node_count);
	if (vars->z_values == NULL)
		exit(1);
}

void	init_mlx(t_vars_new *vars)
{
	vars->mlx = mlx_init(1920, 1080, "FdF", true);
	vars->img = mlx_new_image(vars->mlx, 1920, 1080);
}

void	start_mlx(t_vars_new *vars)
{
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
	mlx_loop_hook(vars->mlx, &hook, vars);
	mlx_loop(vars->mlx);
}

void	init_vars(t_vars_new *vars, int argc, char **argv)
{
	vars->scale = 20;
	vars->argc = argc;
	vars->argv = argv;
	vars->i_z = 0;
	vars->angle = 0.028;
	vars->acc_angle = 0;
	vars->acc_angle_y = 0;
	vars->zoom = 1;
	vars->move_x = 0;
	vars->move_y = 0;
	vars->color = 0xFFFFFF;
}

void	init_params(t_vars_new *vars)
{
	vars->scale = 20;
	vars->l = vars->length * vars->scale;
	vars->w = vars->width * vars->scale;
}

int	main(int argc, char **argv)
{
	t_vars_new vars_new;
	int	node_count;

	vars_new.width = get_width_and_length(argc, argv, &vars_new);
	node_count = vars_new.width * vars_new.length;
	allocate_value_arrays(&vars_new, node_count);
	init_mlx(&vars_new);
	init_params(&vars_new);
	init_vars(&vars_new, argc, argv);
	adjust_scale(&vars_new, &vars_new.l, &vars_new.w);
	draw_map(&vars_new, argc, argv);
	start_mlx(&vars_new);
	return (0);
}

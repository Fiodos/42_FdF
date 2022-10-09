/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:59:52 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 10:42:47 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/utils.h"
#include "../lib/operations.h"
#include "../lib/nodes.h"

void	reset_coordinates(int *x, int *y, t_vars_new *vars)
{
	*x -= (vars->length * vars->scale);
	*y += vars->scale;
}

t_vec	*create_list(int fd, t_vars_new *vars, int x, int y)
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
		free_split_line(ps, line);
		reset_coordinates(&x, &y, vars);
		line = get_next_line(fd);
	}
	close(fd);
	return (vec);
}

void	create_zoomed_frame(t_vars_new *vars)
{
	t_vec	*vec;
	double	before_x;
	double	before_y;
	double	after_x;
	double	after_y;

	vec = vars->head->next;
	before_x = vars->head->next->x;
	before_y = vars->head->next->y;
	while (vec)
	{
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

t_vec	*create_default_frame(t_vars_new *vars, int argc, char **argv)
{
	t_vec	*vec;
	int		fd;
	int		x;
	int		y;

	fd = open_file(argc, argv);
	x = (1920 / 2);
	y = (HEIGHT / 2);
	vec = create_list(fd, vars, x, y);
	init_pos_list(vec, vars);
	lstiter(vec, &rotate);
	lstiter_rx(vec, &rotate_around_x);
	return (vec);
}

void	create_new_frame(t_vars_new *vars, int id)
{
	t_vec	*vec;

	if (vars->projection == 0)
	{
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
	else
	{
		set_initial_values(vars, id);
		if (vars->zoom > 1 || vars->zoom < 1)
			create_zoomed_frame(vars);
		vec = vars->head->next;
		move_operation(vars, vec);
	}
}

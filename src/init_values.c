/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_values.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:49:10 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 12:09:36 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/nodes.h"
#include "../lib/utils.h"
#include "../lib/operations.h"

void	init_pos_list(t_vec *vec, t_vars_new *vars)
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
	if (vars->projection == 0)
	{
		lstiter(vec, &rotate);
		lstiter_rx(vec, &rotate_around_x);
	}
}

void	init_vars(t_vars_new *vars, int argc, char **argv)
{
	vars->scale = 20;
	vars->width = 0;
	vars->length = 0;
	vars->argc = argc;
	vars->argv = argv;
	vars->i_z = 0;
	vars->angle = 0.028;
	vars->acc_angle = 0;
	vars->acc_angle_y = 0;
	vars->zoom = 1;
	vars->move_x = 0;
	vars->move_y = 0;
	vars->projection = 0;
	vars->color = WHITE;
}

void	init_params(t_vars_new *vars)
{
	vars->scale = 20;
	vars->l_scaled = vars->length * vars->scale;
	vars->w_scaled = vars->width * vars->scale;
}

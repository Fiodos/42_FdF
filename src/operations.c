/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:43:03 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/08 19:54:13 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/operations.h"

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

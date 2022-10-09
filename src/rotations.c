/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotations.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:39:43 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/08 20:40:04 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"

void	rotate(t_vec *vec, int x, int y)
{
	double	x_new;
	double	y_new;

	x_new = (vec->x - x) * cos(PI / 4) - (vec->y - y) * sin(PI / 4);
	y_new = (vec->x - x) * sin(PI / 4) + (vec->y - y) * cos(PI / 4);
	vec->x = x_new + x;
	vec->y = y_new + y;
}

void	rotate_around_x(t_vec *vec)
{
	double	y_new;
	double	z_new;

	y_new = (vec->y * cos(PI / 4)) - (vec->z * sin(PI / 4));
	z_new = (vec->y * sin(PI / 4) + (vec->z) * cos(PI / 4));
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

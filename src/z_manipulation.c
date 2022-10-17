/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_manipulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:52:01 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/17 10:31:49 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/utils.h"

int	get_z(t_vars_new *vars, char **split, char *line)
{
	int	z;

	z = ft_atoi_fdf(split, vars, line);
	if (z > 30 || z < -30)
		z *= 2;
	else
		z *= vars->scale;
	vars->z_values[vars->i_z] = z;
	vars->i_z++;
	return (z);
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
	if (id == 0)
		increase_z(vars, vec, i);
	else if (id == 1)
		decrease_z(vars, vec, i);
	else
		vec->z = vars->z_values[i];
}

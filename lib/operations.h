/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:23:45 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 12:24:54 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATIONS_H
# define OPERATIONS_H

# include "fdf.h"

void	rotate(t_vec *vec, int x, int y);

void	rotate_operation_y(t_vars_new *vars, t_vec *vec);

void	rotate_around_y_custom_angle(t_vec *vec, double angle);

void	rotate_around_x(t_vec *vec);

void	rotate_operation_x(t_vars_new *vars, t_vec *vec);

void	rotate_around_x_custom_angle(t_vec *vec, double angle);

void	move_operation(t_vars_new *vars, t_vec *vec);

void	increase_z(t_vars_new *vars, t_vec *vec, int i);

void	decrease_z(t_vars_new *vars, t_vec *vec, int i);
#endif

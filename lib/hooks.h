/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:20:22 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 12:24:47 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HOOKS_H
# define HOOKS_H

# include "fdf.h"

void	rotate_x_hook(t_vars_new *vars);

void	rotate_y_hook(t_vars_new *vars);

void	change_projection_hook(mlx_key_data_t pressed, void *vars);

void	change_z_hook(t_vars_new *vars);

void	move_hook_sideways(t_vars_new *vars);

void	move_hook_up_down(t_vars_new *vars);

void	zoom_hook(t_vars_new *vars);

void	hook(void *param);
#endif

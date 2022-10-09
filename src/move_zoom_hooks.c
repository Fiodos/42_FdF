/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_zoom_hooks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:46:06 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 10:42:50 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/draw.h"
#include "../lib/utils.h"

void	move_hook_sideways(t_vars_new *vars)
{
	mlx_delete_image(vars->mlx, vars->img);
	vars->img = mlx_new_image(vars->mlx, WIDTH, HEIGHT);
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
	vars->img = mlx_new_image(vars->mlx, WIDTH, HEIGHT);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_UP))
		vars->move_y -= 10;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_DOWN))
		vars->move_y += 10;
	create_new_frame(vars, 2);
	simple_draw(vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
}

void	zoom_hook(t_vars_new *vars)
{
	mlx_delete_image(vars->mlx, vars->img);
	vars->img = mlx_new_image(vars->mlx, WIDTH, WIDTH);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_EQUAL))
		vars->zoom *= 1.1;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_MINUS))
		vars->zoom /= 1.1;
	create_new_frame(vars, 2);
	simple_draw(vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
}

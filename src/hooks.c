/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:44:01 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 10:42:48 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/utils.h"
#include "../lib/hooks.h"
#include "../lib/draw.h"

void	rotate_x_hook(t_vars_new *vars)
{
	mlx_delete_image(vars->mlx, vars->img);
	vars->img = mlx_new_image(vars->mlx, WIDTH, HEIGHT);
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
	vars->img = mlx_new_image(vars->mlx, WIDTH, HEIGHT);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_A))
		vars->acc_angle_y += vars->angle;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_D))
		vars->acc_angle_y -= vars->angle;
	create_new_frame(vars, 2);
	simple_draw(vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
}

void	change_projection_hook(mlx_key_data_t pressed, void *vars)
{
	t_vars_new	*vars_new;

	vars_new = (t_vars_new *)vars;
	pressed.key = MLX_KEY_P;
	if (mlx_is_key_down(vars_new->mlx, pressed.key))
	{
		mlx_delete_image(vars_new->mlx, vars_new->img);
		vars_new->img = mlx_new_image(vars_new->mlx, WIDTH, HEIGHT);
		if (vars_new->projection == 0)
			vars_new->projection = 1;
		else
			vars_new->projection = 0;
		create_new_frame(vars, 2);
		simple_draw(vars);
		mlx_image_to_window(vars_new->mlx, vars_new->img, 0, 0);
	}
}

void	change_z_hook(t_vars_new *vars)
{
	mlx_delete_image(vars->mlx, vars->img);
	vars->img = mlx_new_image(vars->mlx, WIDTH, HEIGHT);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_1))
		create_new_frame(vars, 0);
	if (mlx_is_key_down(vars->mlx, MLX_KEY_2))
		create_new_frame(vars, 1);
	simple_draw(vars);
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
}

void	hook(void *param)
{
	t_vars_new	*vars;

	vars = (t_vars_new *)param;
	if (mlx_is_key_down(vars->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(vars->mlx);
	else if (mlx_is_key_down(vars->mlx, MLX_KEY_EQUAL)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_MINUS))
		zoom_hook(vars);
	else if (mlx_is_key_down(vars->mlx, MLX_KEY_W)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_S))
		rotate_x_hook(vars);
	else if (mlx_is_key_down(vars->mlx, MLX_KEY_A)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_D))
		rotate_y_hook(vars);
	else if (mlx_is_key_down(vars->mlx, MLX_KEY_1)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_2))
		change_z_hook(vars);
	else if (mlx_is_key_down(vars->mlx, MLX_KEY_LEFT)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_RIGHT))
		move_hook_sideways(vars);
	else if (mlx_is_key_down(vars->mlx, MLX_KEY_UP)
		|| mlx_is_key_down(vars->mlx, MLX_KEY_DOWN))
		move_hook_up_down(vars);
}

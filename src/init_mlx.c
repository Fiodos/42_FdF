/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:51:11 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 10:42:49 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/hooks.h"

void	init_mlx(t_vars_new *vars)
{
	vars->mlx = mlx_init(WIDTH, HEIGHT, "FdF", false);
	vars->img = mlx_new_image(vars->mlx, WIDTH, HEIGHT);
}

void	start_mlx(t_vars_new *vars)
{
	mlx_image_to_window(vars->mlx, vars->img, 0, 0);
	mlx_loop_hook(vars->mlx, &hook, vars);
	mlx_loop(vars->mlx);
}

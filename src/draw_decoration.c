/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 15:48:52 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 16:19:09 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"

void	draw_menu(t_vars_new *vars)
{
	mlx_put_string(vars->mlx, "---------------------------", 50, 60);
	mlx_put_string(vars->mlx, "MENU", 50, 40);
	mlx_put_string(vars->mlx, "rotation x-axis ==> W && S", 50, 80);
	mlx_put_string(vars->mlx, "rotation y-axis ==> A && D", 50, 110);
	mlx_put_string(vars->mlx, "movement ==> arrow_keys", 50, 140);
	mlx_put_string(vars->mlx, "altitude ==> 1 && 2", 50, 170);
	mlx_put_string(vars->mlx, "zoom ==> - && =", 50, 200);
	mlx_put_string(vars->mlx, "parallel projection ==> P", 50, 230);
}

void	draw_42(t_vars_new *vars)
{
	mlx_put_string(vars->mlx, "        :::      ::::::::", 20, 500);
	mlx_put_string(vars->mlx, "      :+:      :+:    :+:", 20, 520);
	mlx_put_string(vars->mlx, "    +:+ +:+         +:+", 20, 540);
	mlx_put_string(vars->mlx, "  +#+  +:+       +#+", 20, 560);
	mlx_put_string(vars->mlx, "+#+#+#+#+#+   +#+        ", 20, 580);
	mlx_put_string(vars->mlx, "     #+#    #+#          ", 20, 600);
	mlx_put_string(vars->mlx, "     ###   ########.HN    ", 20, 620);
}

void	draw_decoration(t_vars_new *vars)
{
	draw_menu(vars);
	draw_42(vars);
}

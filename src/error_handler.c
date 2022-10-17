/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:29:52 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/17 10:33:05 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/draw.h"
#include "../lib/nodes.h"
#include "../lib/utils.h"
#include "../lib/operations.h"

void	error_handler(t_vars_new *vars, char **split, char *line)
{
	perror("Invalid map");
	free_all(vars->head, vars);
	free_split_line(split, line);
	mlx_delete_image(vars->mlx, vars->img);
	mlx_terminate(vars->mlx);
	exit(1);
}

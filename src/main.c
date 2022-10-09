/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:48:24 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 12:11:20 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/hooks.h"
#include "../lib/draw.h"
#include "../lib/nodes.h"
#include "../lib/operations.h"
#include "../lib/utils.h"

int	main(int argc, char **argv)
{
	t_vars_new	vars;
	int			node_count;

	get_width_and_length(argc, argv, &vars);
	node_count = vars.width * vars.length;
	allocate_value_arrays(&vars, node_count);
	init_mlx(&vars);
	init_params(&vars);
	init_vars(&vars, argc, argv);
	adjust_scale(&vars, &vars.l_scaled, &vars.w_scaled);
	draw_map(&vars, argc, argv);
	mlx_key_hook(vars.mlx, &change_projection_hook, &vars);
	start_mlx(&vars);
	free_all(vars.head->next, &vars);
	return (0);
}

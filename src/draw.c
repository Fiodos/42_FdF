/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:41:12 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/08 20:38:10 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/nodes.h"
#include "../lib/draw.h"
#include "../lib/utils.h"

void	draw_to_next_row(t_vec *vec, mlx_image_t *img, int length)
{
	t_vec	*next_vec;
	int		i;

	next_vec = vec;
	i = 0;
	while (i < length)
	{
		next_vec = next_vec->next;
		if (next_vec == NULL)
			return ;
		i++;
	}
	bresenham(img, vec, next_vec);
}

void	draw_map(t_vars_new *vars, int argc, char **argv)
{
	t_vec	*vec;
	t_vec	*next_vec;
	int		i;
	int		eor;

	vec = create_default_frame(vars, argc, argv);
	vars->head = vec;
	vec = vec->next;
	next_vec = vec->next;
	i = 0;
	while (next_vec)
	{
		bresenham(vars->img, vec, next_vec);
		manage_row(&vec, &next_vec);
		i++;
		eor = i % (vars->length - 1);
		if (!eor && next_vec != NULL)
			manage_row(&vec, &next_vec);
	}
	manage_nodes(vars->head, &vec, &next_vec);
	while (next_vec)
	{
		draw_to_next_row(vec, vars->img, vars->length);
		manage_row(&vec, &next_vec);
	}
}

void	simple_draw(t_vars_new *vars)
{
	int		i;
	int		eor;
	t_vec	*vec;
	t_vec	*next_vec;

	i = 0;
	vec = vars->head->next;
	next_vec = vec->next;
	while (next_vec)
	{
		bresenham(vars->img, vec, next_vec);
		manage_row(&vec, &next_vec);
		i++;
		eor = i % (vars->length - 1);
		if (!eor && next_vec != NULL)
			manage_row(&vec, &next_vec);
	}
	manage_nodes(vars->head, &vec, &next_vec);
	while (next_vec)
	{
		draw_to_next_row(vec, vars->img, vars->length);
		manage_row(&vec, &next_vec);
	}
}

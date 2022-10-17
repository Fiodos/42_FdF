/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:06:12 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 16:20:07 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_H
# define DRAW_H

# include "fdf.h"

void	get_d_more_than_one(t_vec *vec, t_vec *next_vec, int *y, int *d);

void	get_d_less_than_one(t_vec *vec, t_vec *next_vec, int *x, int *d);

void	bresen_pos_slope(t_vec *vec, t_vec *next_vec, mlx_image_t *img);

void	bresen_neg_slope(t_vec *vec, t_vec *next_vec, mlx_image_t *img);

void	bresenham(mlx_image_t *img, t_vec *vec, t_vec *next_vec);

void	draw_to_next_row(t_vec *vec, mlx_image_t *img, int length);

void	draw_map(t_vars_new *vars, int argc, char **argv);

void	simple_draw(t_vars_new *vars);

void	draw_menu(t_vars_new *vars);

void	draw_42(t_vars_new *vars);

void	draw_decoration(t_vars_new *vars);

int		get_dx(t_vec *vec, t_vec *next_vec);

int		get_dy(t_vec *vec, t_vec *next_vec);

int		abs(int dx);
#endif

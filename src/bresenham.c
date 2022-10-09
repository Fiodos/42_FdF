/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:38:33 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 12:19:29 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/draw.h"
#include "../lib/utils.h"

void	get_d_more_than_one(t_vec *vec, t_vec *next_vec, int *y, int *d)
{
	if (get_dy(vec, next_vec) < 0)
		*y = *y - 1;
	else
		*y = *y + 1;
	*d += (2 * abs(get_dy(vec, next_vec)) - 2 * abs(get_dx(vec, next_vec)));
}

void	get_d_less_than_one(t_vec *vec, t_vec *next_vec, int *x, int *d)
{
	if (get_dx(vec, next_vec) < 0)
		*x = *x - 1;
	else
		*x = *x + 1;
	*d += 2 * abs(get_dx(vec, next_vec)) - 2 * abs(get_dy(vec, next_vec));
}

void	bresen_pos_slope(t_vec *vec, t_vec *next_vec, mlx_image_t *img)
{
	int	d;
	int	i;
	int	x;
	int	y;

	d = 2 * abs(get_dy(vec, next_vec)) - abs(get_dx(vec, next_vec));
	i = 0;
	x = (int)round(vec->x);
	y = (int)round(vec->y);
	while (i < abs(get_dx(vec, next_vec)))
	{
		if (get_dx(vec, next_vec) < 0)
			x = x - 1;
		else
			x = x + 1;
		if (d < 0)
			d = d + 2 * abs(get_dy(vec, next_vec));
		else
			get_d_more_than_one(vec, next_vec, &y, &d);
		if (check_range(x, y))
			mlx_put_pixel(img, x, y, vec->color);
		i++;
	}
}

void	bresen_neg_slope(t_vec *vec, t_vec *next_vec, mlx_image_t *img)
{
	int	d;
	int	i;
	int	x;
	int	y;

	d = 2 * abs(get_dy(vec, next_vec)) - abs(get_dx(vec, next_vec));
	i = 0;
	x = (int)round(vec->x);
	y = (int)round(vec->y);
	while (i < abs(get_dy(vec, next_vec)))
	{
		if (get_dy(vec, next_vec) < 0)
			y = y - 1;
		else
			y = y + 1;
		if (d < 0)
			d = d + 2 * abs(get_dx(vec, next_vec));
		else
			get_d_less_than_one(vec, next_vec, &x, &d);
		if (check_range(x, y))
			mlx_put_pixel(img, x, y, vec->color);
		i++;
	}
}

void	bresenham(mlx_image_t *img, t_vec *vec, t_vec *next_vec)
{
	int	dx;
	int	dy;

	dx = (int)next_vec->x - (int)vec->x;
	dy = (int)next_vec->y - (int)vec->y;
	if (abs(dx) > abs(dy))
		bresen_pos_slope(vec, next_vec, img);
	else
		bresen_neg_slope(vec, next_vec, img);
}

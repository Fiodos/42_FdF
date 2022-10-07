/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 17:03:26 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/07 20:03:52 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdio.h>
# include <stdlib.h>
# include "../../MLX42/include/MLX42/MLX42.h"

#define PI 3.142

typedef struct s_vec
{
	double		x;
	double		y;
	double		z;
	uint32_t	color;
	struct s_vec *next;
}	t_vec;

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct s_vars_new {
	mlx_t 		*mlx;
	mlx_image_t *img;
	t_vec 		*head;
	char		**argv;
	int			scale;
	int			length;
	int			width;
	int			argc;
	int			move_x;
	int			move_y;
	int			i_z;
	double		l;
	double		w;
	double		*x_values;
	double		*y_values;
	double		*z_values;
	double		angle;
	double		acc_angle;
	double		acc_angle_y;
	double		zoom;
	uint32_t	color;
}				t_vars_new;

t_vec	*new_vec(int x, int y, int z);

t_vec	*lstlast(t_vec *lst);

void	lstadd_back(t_vec **tmp, t_vec **next, t_vars_new *vars);

void	lstiter(t_vec *lst, void (*f)(t_vec *, int, int));

void	lstiter_rx(t_vec *lst, void (*f)(t_vec *));

#endif

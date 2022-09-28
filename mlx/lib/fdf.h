/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 17:03:26 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/09/28 20:36:28 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdio.h>
# include <stdlib.h>
#include "mlx.h"

#define PI 3.142

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
	struct s_vec *next;
}	t_vec;

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_vars {
	t_data	img;
	t_vec 	*head;
	void	*mlx;
	void	*win;
	int		scale;
	int		length;
	int		width;
	int		argc;
	double		l;
	double		w;
	char	**argv;
	double		x_values[14000];
	double		y_values[14000];
	double		z_values[14000];
	int			operations[5000];
	int			index;
	double		angle;
	int		rota_counter_x;
	int		rota_counter_x_back;
	int		rota_counter_y;
	int		rota_counter_y_back;
	int		zoom_counter;
	int		neg_zoom_counter;
	int		zoom_in_amount;
	int		zoom_out_amount;
	int		i_z;
	double	inc_z_scale;
	double	dec_z_scale;
}				t_vars;

t_vec	*new_vec(int x, int y, int z);

t_vec	*lstlast(t_vec *lst);

void	lstadd_back(t_vec **lst, t_vec *vec);

void	lstiter(t_vec *lst, void (*f)(t_vec *, int, int), t_vars *vars);

void	lstiter_rx(t_vec *lst, void (*f)(t_vec *));

#endif

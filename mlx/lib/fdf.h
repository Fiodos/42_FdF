/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 17:03:26 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/09/20 15:20:12 by fyuzhyk          ###   ########.fr       */
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
	float	x;
	float	y;
	float	z;
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
	float		l;
	float		w;
	char	**argv;
	float		x_values[5000];
	float		y_values[5000];
	float		z_values[5000];
	float		x_values_dyn[5000];
	float		y_values_dyn[5000];
	float		z_values_dyn[5000];
	int		rota_counter;
	int		i_x;
	int		i_y;
	int		i_z;
}				t_vars;

t_vec	*new_vec(int x, int y, int z);

t_vec	*lstlast(t_vec *lst);

void	lstadd_back(t_vec **lst, t_vec *vec);

void	lstiter(t_vec *lst, void (*f)(t_vec *, int, int), t_vars *vars);

void	lstiter_rx(t_vec *lst, void (*f)(t_vec *));

#endif

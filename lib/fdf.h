/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 17:03:26 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 12:23:20 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include "libft/libft.h"
# include "MLX42/include/MLX42/MLX42.h"

# define PI 3.142
# define WIDTH 1280
# define HEIGHT 700
# define WHITE 0xF0FFFFFF

typedef struct s_vec
{
	uint32_t		color;
	double			x;
	double			y;
	double			z;
	struct s_vec	*next;
}				t_vec;

typedef struct s_vars_new {
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_vec		*head;
	uint32_t	color;
	char		**argv;
	int			scale;
	int			length;
	int			width;
	int			argc;
	int			move_x;
	int			move_y;
	int			i_z;
	int			projection;
	double		l_scaled;
	double		w_scaled;
	double		*x_values;
	double		*y_values;
	double		*z_values;
	double		angle;
	double		acc_angle;
	double		acc_angle_y;
	double		zoom;
}				t_vars_new;
#endif

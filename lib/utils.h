/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:34:09 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/08 19:35:04 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "fdf.h"

void	reset_coordinates(int *x, int *y, t_vars_new *vars);

void	create_zoomed_frame(t_vars_new *vars);

void	create_new_frame(t_vars_new *vars, int id);

void	free_split(char **ps);

void	free_split_line(char **ps, char *line);

void	free_all(t_vec *vec, t_vars_new *vars);

void	init_mlx(t_vars_new *vars);

void	start_mlx(t_vars_new *vars);

void	init_pos_list(t_vec *vec, t_vars_new *vars);

void	set_initial_values(t_vars_new *vars, int id);

void	init_vars(t_vars_new *vars, int argc, char **argv);

void	init_params(t_vars_new *vars);

void	allocate_value_arrays(t_vars_new *vars, int node_count);

void	adjust_scale(t_vars_new *vars, double *length, double *width);

void	get_width_and_length(int argc, char **argv, t_vars_new *vars);

void	define_z_value(t_vars_new *vars, t_vec *vec, int id, int i);

int		hex_to_decimal(const char *color);

int		ft_atoi_fdf(const char *str, t_vars_new *vars);

int		open_file(int argc, char **argv);

int		check_range(int x, int y);

int		get_z(t_vars_new *vars, char **split);

t_vec	*create_list(int fd, t_vars_new *vars, int x, int y);

t_vec	*create_default_frame(t_vars_new *vars, int argc, char **argv);
#endif

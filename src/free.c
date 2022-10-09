/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:54:22 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/08 19:48:22 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"

void	free_split(char **ps)
{
	char	**split;

	split = ps;
	while (*split)
	{
		free(*split);
		split++;
	}
	free(ps);
	ps = NULL;
}

void	free_split_line(char **ps, char *line)
{
	free_split(ps);
	free(line);
}

void	free_all(t_vec *vec, t_vars_new *vars)
{
	t_vec	*tmp;
	int		i;

	i = 0;
	while (vec != NULL)
	{
		tmp = vec;
		vec = vec->next;
		free(tmp);
	}
	free(vars->x_values);
	free(vars->y_values);
	free(vars->z_values);
}

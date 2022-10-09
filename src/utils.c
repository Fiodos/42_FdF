/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:55:26 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 13:26:52 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/utils.h"

void	allocate_value_arrays(t_vars_new *vars, int node_count)
{
	vars->x_values = malloc(sizeof(double) * node_count);
	if (vars->x_values == NULL)
	{
		perror("Malloc failed");
		exit(errno);
	}
	vars->y_values = malloc(sizeof(double) * node_count);
	if (vars->y_values == NULL)
	{
		perror("Malloc failed");
		exit(errno);
	}
	vars->z_values = malloc(sizeof(double) * node_count);
	if (vars->z_values == NULL)
	{
		perror("Malloc failed");
		exit(errno);
	}
}

void	adjust_scale(t_vars_new *vars, double *length, double *width)
{
	if (*length >= 600)
		vars->scale = vars->scale / 4;
	if (*length >= 10000)
		vars->scale = 2;
	*length = vars->length * vars->scale;
	*width = vars->width * vars->scale;
}

void	get_width_and_length(int argc, char **argv, t_vars_new *vars)
{
	int		fd;
	char	*line;
	char	**split;

	fd = open_file(argc, argv);
	line = get_next_line(fd);
	if (line != NULL)
	{
		split = ft_split(line, ' ');
		while (split[vars->length])
			vars->length++;
		free_split(split);
	}
	while (line)
	{
		vars->width++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

int	open_file(int argc, char **argv)
{
	int	fd;

	if (argc < 2)
	{
		perror("Invalid execution");
		exit(errno);
	}
	fd = open(argv[argc - 1], O_RDONLY);
	if (fd == -1)
	{
		perror("Could not open the file");
		exit(errno);
	}
	return (fd);
}

int	check_range(int x, int y)
{
	if (x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
		return (1);
	return (0);
}

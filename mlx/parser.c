/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 14:47:25 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/08/28 15:20:01 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "lib/fdf.h"
#include "../libft/libft.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

// t_vec	*create_frame_new(int x_c, int y_c, int fd)
// {
// 	t_vec	*vec; //head of the list;
// 	char	*line;
// 	char	**ps;
// 	int		i;
// 	int		z;

// 	vec = new_vec(0, 0, 0);
// 	line = get_next_line(fd);
// 	while (line)
// 	{
// 		i = 0;
// 		ps = ft_split(line, ' ');
// 		while (ps[i])
// 		{
// 			z = ft_atoi(ps[i]);
// 			lstadd_back(&vec, new_vec(x_c, y_c, z));
// 			x_c += 30;
// 			i++;
// 		}
// 		x_c = x_c - (i * 30); // set x back to the initial value;
// 		y_c += 30;
// 		line = get_next_line(fd);
// 	}
// 	return (vec);
// }

// int main(int argc, char **argv)
// {
// 	int fd = open(argv[argc - 1], O_RDONLY);
// 	int x_c = 1920 / 2; // c = center;
// 	int y_c = 1080 / 2;

// 	t_vec *head = create_frame_new(x_c, y_c, fd);

// 	return (0);
// }

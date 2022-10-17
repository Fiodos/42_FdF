/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_fdf.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 11:02:06 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/13 12:42:54 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"

char	*get_next_line_fdf(int fd)
{
	char	*str;
	int		i;

	str = get_next_line(fd);
	i = 0;
	if (str != NULL)
	{
		while (str[i] != '\n')
			i++;
		str[i] = '\0';
	}
	return (str);
}

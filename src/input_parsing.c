/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:57:07 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/17 10:30:44 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"
#include "../lib/utils.h"

int	hex_to_decimal(const char *color)
{
	char		*new_color;
	long		converted_color;
	int			index;
	int			len;
	int			i;

	converted_color = 0;
	new_color = ft_strjoin(color, "FF");
	len = ft_strlen(new_color) - 1;
	i = 0;
	while (new_color[i])
	{
		index = 0;
		while (new_color[i] != "0123456789abcdef"[index]
			&& new_color[i] != "0123456789ABCDEF"[index])
			index++;
		converted_color = converted_color + (index * pow(16, len));
		len--;
		i++;
	}
	free(new_color);
	return ((int)converted_color);
}

int	ft_atoi_fdf(char **split, t_vars_new *vars, char *line)
{
	int	i;
	int	minus_count;
	int	num;

	i = 0;
	minus_count = 1;
	num = 0;
	while (split[vars->length][i] == '\r' || split[vars->length][i] == '\t'
		|| split[vars->length][i] == ' ' || split[vars->length][i] == '\f'
		|| split[vars->length][i] == '\v' || split[vars->length][i] == '\n')
		i++;
	if (split[vars->length][i] == '-')
	{
		minus_count *= (-1);
		i++;
	}
	else if (split[vars->length][i] == '+')
		i++;
	while (split[vars->length][i] <= '9' && split[vars->length][i] >= '0')
		num = (split[vars->length][i++] - '0') + (num * 10);
	if (split[vars->length][i] == ',')
		vars->color = hex_to_decimal(&split[vars->length][i + 3]);
	if (i == 0 && num == 0)
		error_handler(vars, split, line);
	return (num * minus_count);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:57:07 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/08 20:40:36 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"

int	hex_to_decimal(const char *color)
{
	char	*new_color;
	int		converted_color;
	int		index;
	int		len;
	int		i;

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
	return (converted_color);
}

int	ft_atoi_fdf(const char *str, t_vars_new *vars)
{
	int	i;
	int	minus_count;
	int	num;

	i = 0;
	minus_count = 1;
	num = 0;
	while (str[i] == '\r' || str[i] == '\t' || str[i] == ' '
		|| str[i] == '\f' || str[i] == '\v' || str[i] == '\n')
		i++;
	if (str[i] == '-')
	{
		minus_count *= (-1);
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] <= '9' && str[i] >= '0')
	{
		num = (str[i] - '0') + (num * 10);
		i++;
	}
	if (str[i] == ',')
		vars->color = hex_to_decimal(&str[i + 3]);
	return (num * minus_count);
}

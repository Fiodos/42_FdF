/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:53:28 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 12:10:30 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"

t_vec	*new_vec(int x, int y, int z)
{
	t_vec	*result;

	result = malloc(sizeof(t_vec));
	if (result == NULL)
		return (NULL);
	result->x = (double)x;
	result->y = (double)y;
	result->z = (double)z;
	result->next = NULL;
	return (result);
}

t_vec	*lstlast(t_vec *lst)
{
	t_vec	*curr;

	curr = lst;
	if (curr == 0)
		return (curr);
	while (curr->next != NULL)
		curr = curr->next;
	return (curr);
}

void	lstadd_back(t_vec **tmp, t_vec **next, t_vars_new *vars)
{
	(*next)->color = vars->color;
	(*tmp)->next = *next;
	*tmp = *next;
	vars->length++;
	vars->color = WHITE;
}

void	lstiter(t_vec *lst, void (*f)(t_vec *, int, int))
{
	t_vec	*curr;
	int		x;
	int		y;

	x = WIDTH / 2;
	y = HEIGHT / 2;
	curr = lst;
	while (curr != NULL)
	{
		f(curr, x, y);
		curr = curr->next;
	}
}

void	lstiter_rx(t_vec *lst, void (*f)(t_vec *))
{
	t_vec	*curr;

	curr = lst;
	while (curr != NULL)
	{
		f(curr);
		curr = curr->next;
	}
}

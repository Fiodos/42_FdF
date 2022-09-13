/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:53:28 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/09/13 13:31:38 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/fdf.h"

t_vec	*new_vec(int x, int y, int z)
{
	t_vec	*result;

	result = malloc(sizeof(t_vec));
	if (result == NULL)
		return (NULL);
	result->x = x;
	result->y = y;
	result->z = z;
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

void	lstadd_back(t_vec **lst, t_vec *new)
{
	t_vec	*last;

	if (*lst == 0)
	{
		*lst = new;
		new->next = NULL;
		return ;
	}
	last = lstlast(*lst);
	last->next = new;
	new = lstlast(new);
	new->next = NULL;
}

void	lstiter(t_vec *lst, void (*f)(t_vec *, int, int))
{
	t_vec	*curr;
	int		x = 0;
	int		y = 0;

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

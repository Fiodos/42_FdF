/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:35:58 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/08 18:37:52 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/fdf.h"

void	get_next_nodes(t_vec **vec, t_vec **next_vec)
{
	*vec = (*vec)->next;
	*next_vec = (*vec)->next;
}

void	reset_nodes(t_vec **vec, t_vec **next_vec, t_vars_new *vars)
{
	*vec = vars->head->next;
	*next_vec = (*vec)->next;
}

void	manage_nodes(t_vec *head, t_vec **vec, t_vec **next_vec)
{
	*vec = head->next;
	*next_vec = (*vec)->next;
}

void	manage_row(t_vec **vec, t_vec **next_vec)
{
	*vec = (*vec)->next;
	*next_vec = (*vec)->next;
}

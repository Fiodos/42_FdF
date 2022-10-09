/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:15:15 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/10/09 12:24:50 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODES_H
# define NODES_H

# include "fdf.h"

t_vec	*new_vec(int x, int y, int z);

t_vec	*lstlast(t_vec *lst);

void	lstadd_back(t_vec **tmp, t_vec **next, t_vars_new *vars);

void	lstiter(t_vec *lst, void (*f)(t_vec *, int, int));

void	lstiter_rx(t_vec *lst, void (*f)(t_vec *));

void	get_next_nodes(t_vec **vec, t_vec **next_vec);

void	reset_nodes(t_vec **vec, t_vec **next_vec, t_vars_new *vars);

void	manage_nodes(t_vec *head, t_vec **vec, t_vec **next_vec);

void	manage_row(t_vec **vec, t_vec **next_vec);
#endif

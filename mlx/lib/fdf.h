/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 17:03:26 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/09/13 13:31:56 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdio.h>
# include <stdlib.h>

#define PI 3.142

typedef struct s_vec
{
	int	x;
	int	y;
	int	z;
	struct s_vec *next;
}	t_vec;

t_vec	*new_vec(int x, int y, int z);

t_vec	*lstlast(t_vec *lst);

void	lstadd_back(t_vec **lst, t_vec *vec);

void	lstiter(t_vec *lst, void (*f)(t_vec *, int, int));

void	lstiter_rx(t_vec *lst, void (*f)(t_vec *));

#endif

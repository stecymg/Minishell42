/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:19:53 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/30 08:08:18 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*new_node(char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		set_err_malloc("parse\n");
		return (NULL);
	}
	init_token(new);
	new->value = value;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

int	push_back(t_data *data, char *value)
{
	t_token	*new;
	t_token	*tmp;

	new = new_node(value);
	if (new == NULL)
		return (-1);
	tmp = data->head;
	if (data->head == NULL)
	{
		data->head = new;
		return (0);
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
	return (0);
}

int	ft_size(t_token *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

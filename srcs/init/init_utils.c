/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:28:25 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:19:59 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	to_export(t_export *export)
{
	char		*str;
	t_export	*tmp;

	tmp = export->next;
	while (tmp)
	{
		if (ft_strcmp(export->name, tmp->name) > 0)
		{
			str = export->name;
			export->name = tmp->name;
			tmp->name = str;
			str = export->value;
			export->value = tmp->value;
			tmp->value = str;
		}
		tmp = tmp->next;
	}
}

t_export	*init_export(char **env)
{
	t_export	*export;
	t_export	*tmp;
	int			count;

	export = new_export(env[0]);
	if (export == NULL)
		return (err_malloc_return("export\n", NULL));
	if (env[0] == NULL)
		return (export);
	tmp = export;
	count = 0;
	while (env[++count])
	{
		export->next = new_export(env[count]);
		if (export->next == NULL)
			return (err_malloc_return("export\n", NULL));
		export = export->next;
	}
	export = tmp;
	while (export)
	{
		to_export(export);
		export = export->next;
	}
	return (tmp);
}

void	init_token(t_token *token)
{
	token->value = NULL;
	token->args = NULL;
	token->red = NULL;
	token->next = NULL;
	token->prev = NULL;
	token->fds[0] = 0;
	token->fds[1] = 1;
}

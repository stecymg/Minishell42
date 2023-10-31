/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 09:10:22 by thsamina          #+#    #+#             */
/*   Updated: 2023/10/31 14:41:20 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == -1)
			str[i] = '\'';
		else if (str[i] == -2)
			str[i] = '\"';
		i++;
	}
}

static int	in_split(t_data *data, t_token *token, int (f)(t_data*, t_token*))
{
	char	*s;

	while (token)
	{
		token->fds[0] = 0;
		token->fds[1] = 1;
		s = ft_strtrim(token->value, " \t\n\v\f\r");
		free(token->value);
		token->value = s;
		if (f(data, token) == -1)
			return (-1);
		token = token->next;
	}
	return (0);
}

static void	give_tab(char **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		replace_quotes(tab[i]);
}

int	split_cmd(t_data *data, t_params *params)
{
	t_token	*tmp;

	if (pipe_split(data) == -1)
		return (-1);
	tmp = data->head;
	if (in_split(data, tmp, is_redir) == -1)
		return (-1);
	tmp = data->head;
	if (expand(tmp, data, params) == -1)
		return (-1);
	tmp = data->head;
	if (in_split(data, tmp, ft_start) == -1)
		return (-1);
	tmp = data->head;
	if (no_quotes(tmp, params) == -1)
		return (-1);
	tmp = data->head;
	while (tmp)
	{
		ft_redir(tmp->red, params, tmp);
		give_tab(tmp->args);
		give_tab(tmp->red);
		tmp = tmp->next;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:58:19 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 14:17:47 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	count_red(char *value, int *count)
{
	int		i;
	char	quote;

	i = 0;
	while (value[i] != '\0')
	{
		if (value[i] == '\'' || value[i] == '\"')
		{
			quote = value[i++];
			while (value[i] != quote)
				i++;
		}
		else if (value[i] == '>' || value[i] == '<')
		{
			if (value[i] == value[i + 1])
				i++;
			(*count)++;
		}
		i++;
	}
}

static void	find_redir(t_data *data, t_token *token)
{
	char	quote;

	while (token->value[data->i] != '>' && token->value[data->i] != '<')
	{
		if (token->value[data->i] == '\'' || token->value[data->i] == '\"')
		{
			quote = token->value[data->i];
			data->i++;
			while (token->value[data->i] != quote)
				data->i++;
		}
		data->i++;
	}
}

static void	no_spaces(t_data *data, t_token *token)
{
	int		tmp;
	char	quote;

	tmp = 0;
	while (ft_space(token->value[data->i]) == 0)
	{
		data->i++;
		tmp++;
	}
	ft_memcpy(&token->value[data->i - tmp], &token->value[data->i],
		ft_strlen(&token->value[data->i]) + 1);
	data->i -= tmp;
	while (token->value[data->i] != '\0' && ft_space(
			token->value[data->i]) != 0 && token->value[data->i] != '>'
		&& token->value[data->i] != '<')
	{
		if (token->value[data->i] == '\'' || token->value[data->i] == '\"')
		{
			quote = token->value[data->i];
			data->i++;
			while (token->value[data->i] != quote)
				data->i++;
		}
		data->i++;
	}
}

static int	get_redir(t_data *data, t_token *token, int count)
{
	int		j;
	int		save;

	j = 0;
	data->i = 0;
	token->red[count] = NULL;
	while (j < count)
	{
		find_redir(data, token);
		save = data->i;
		data->i++;
		if (token->value[data->i] == '>' || token->value[data->i] == '<')
			data->i++;
		no_spaces(data, token);
		token->red[j] = ft_strndup(&token->value[save], data->i - save);
		if (token->red[j] == NULL)
			return (set_err_malloc("parse\n"));
		ft_memcpy(&token->value[save], &token->value[data->i],
			ft_strlen(&token->value[data->i]) + 1);
		data->i = save;
		j++;
	}
	return (0);
}

int	is_redir(t_data *data, t_token *token)
{
	int		count;

	count = 0;
	count_red(token->value, &count);
	token->red = malloc(sizeof(char *) * (count + 1));
	if (token->red == NULL)
		return (set_err_malloc("parse\n"));
	if (get_redir(data, token, count) == -1)
		return (-1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_start.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:22:55 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 14:04:28 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_count_words(t_data *data, char *s)
{
	int		count;
	char	quote;

	count = 0;
	data->i = 0;
	while (s[data->i] != '\0')
	{
		while (s[data->i] != '\0' && ft_space(s[data->i]) != 0)
		{
			if (s[data->i] == '\'' || s[data->i] == '\"')
			{
				quote = s[data->i];
				data->i++;
				while (s[data->i] && s[data->i] != quote)
					data->i++;
				if (s[data->i] == '\0')
					break ;
			}
			data->i++;
		}
		count++;
		while (s[data->i] != '\0' && ft_space(s[data->i]) == 0)
			data->i++;
	}
	return (count);
}

static int	ft_count_char(char *str, int *i)
{
	char	quote;
	int		count;

	count = 0;
	while (str[i[0]] != '\0' && ft_space(
			str[i[0]]) != 0)
	{
		if (str[i[0]] == '\'' || str[i[0]] == '\"')
		{
			quote = str[i[0]];
			count++;
			i[0]++;
			while (str[i[0]] && str[i[0]] != quote)
			{
				count++;
				i[0]++;
			}
		}
		if (str[i[0]])
			i[0]++;
		count++;
	}
	return (count);
}

int	ft_start(t_data *data, t_token *tok)
{
	int		tmp;
	int		iter[4];

	iter[0] = 0;
	iter[1] = 0;
	iter[2] = ft_count_words(data, tok->value);
	tok->args = malloc(sizeof(char *) * (iter[2] + 1));
	if (!tok->args)
		return (set_err_malloc("parse\n"));
	tok->args[iter[2]] = NULL;
	while (iter[1] < iter[2])
	{
		tmp = iter[0];
		iter[3] = ft_count_char(tok->value, &iter[0]);
		if (ft_space(tok->value[iter[0]]) == 0 || tok->value[iter[0]] == '\0')
		{
			tok->args[iter[1]++] = ft_strndup(&tok->value[tmp], iter[3]);
			if (tok->args[iter[1] - 1] == NULL)
				return (set_err_malloc("parse\n"));
		}
		while (tok->value[iter[0]] != '\0'
			&& ft_space(tok->value[iter[0]]) == 0)
			iter[0]++;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:58:19 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:17:55 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check if quotes opened are closed correctly
int	check_quotes(t_data *data)
{
	char	quote;

	data->i = 0;
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == '\'' || data->trimmed[data->i] == '\"')
		{
			quote = data->trimmed[data->i];
			data->i++;
			if (get_next_quote(quote, data) == -1)
				return (-1);
		}
		data->i++;
	}
	return (0);
}

// check if next quote exist
int	get_next_quote(char quote, t_data *data)
{
	while (data->trimmed[data->i] && data->trimmed[data->i] != quote)
		data->i++;
	if (data->trimmed[data->i] == quote)
		return (0);
	return (-1);
}

// suppress quotes
int	move_to_supp(char *str, int j)
{
	char	quote;

	quote = str[j];
	ft_memcpy(&str[j], &str[j + 1], ft_strlen(&str[j + 1]) + 1);
	while (str[j] && str[j] != quote)
		j++;
	ft_memcpy(&str[j], &str[j + 1], ft_strlen(&str[j + 1]) + 1);
	return (j);
}

// find suppressable quotes
int	no_quotes(t_token *token, t_params *params)
{
	t_token	*tmp;
	int		i;
	int		j;

	tmp = token;
	while (tmp)
	{
		i = -1;
		while (tmp->args[++i])
		{
			j = -1;
			while (tmp->args[i][++j] != '\0')
			{
				if (tmp->args[i][j] == '\'' || tmp->args[i][j] == '\"')
					j = move_to_supp(tmp->args[i], j);
				if (tmp->args[i][j] == '\0')
					break ;
				if (tmp->args[i][j] == '\'' || tmp->args[i][j] == '\"')
					j--;
			}
		}
		tmp = tmp->next;
	}
	tmp = token;
	return (quotes_redir(tmp, params));
}

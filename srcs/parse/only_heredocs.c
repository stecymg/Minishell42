/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   only_heredocs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:55:49 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:17:46 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// if parse error, suppress everything except heredocs before error
static int	new_line(t_data *data, int nbr)
{
	int	save;

	save = 0;
	while (data->trimmed[data->i] != '\0'
		&& ft_space(data->trimmed[data->i]) == 0)
	{
		data->i++;
		nbr++;
	}
	while (data->trimmed[data->i] != '\0'
		&& ft_space(data->trimmed[data->i]) != 0)
	{
		save = data->i;
		if (data->trimmed[data->i] == '\''
			|| data->trimmed[data->i] == '\"')
			jump_quotes(data->trimmed, data);
		else
			data->i++;
		nbr += data->i - save;
	}
	return (nbr);
}

// if no delimiter after heredoc with parse error
static int	if_str_zero(char *str, int i)
{
	if (str[i] == '\0')
	{
		str[0] = '\0';
		return (-1);
	}
	return (0);
}

void	only_heredocs(t_data *data, t_params *params)
{
	int	save;
	int	save2;

	params->exit_code = 2;
	data->i -= 1;
	ft_bzero(&data->trimmed[data->i], ft_strlen(&data->trimmed[data->i]));
	data->i = 0;
	save = 0;
	while (data->trimmed[data->i])
	{
		save2 = 2;
		if (data->trimmed[data->i] == '<' && data->trimmed[data->i + 1] == '<')
		{
			ft_memcpy(&data->trimmed[save], &data->trimmed[data->i],
				ft_strlen(&data->trimmed[data->i]) + 2);
			data->i = save + 2;
			if (if_str_zero(data->trimmed, data->i) == -1)
				return ;
			save += new_line(data, save2);
		}
		data->i++;
	}
	data->trimmed[save] = '\0';
}

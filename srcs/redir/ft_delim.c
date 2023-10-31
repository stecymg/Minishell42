/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_delim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 04:15:39 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:21:08 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	in_del_quote_hd(char *str, int i)
{
	char	quote;

	quote = str[i];
	ft_memcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
	while (str[i] != quote)
		i++;
	ft_memcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
	return (i);
}

static char	*del_quotes_hd(char *delim)
{
	int	i;

	i = 0;
	while (delim[i] != '\0')
	{
		if (delim[i] == '\'' || delim[i] == '\"')
			i = in_del_quote_hd(delim, i);
		if (delim[i] == '\0')
			break ;
		if (delim[i] == '\'' || delim[i] == '\"')
			i--;
		i++;
	}
	return (delim);
}

int	delim_quotes(char *delim)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if (delim[i] == '\'' || delim[i] == '\"')
		{
			del_quotes_hd(delim);
			return (1);
		}
		i++;
	}
	return (0);
}

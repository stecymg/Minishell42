/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:53:28 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:15:17 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_error_redir(char *str, char c)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("\'\n", 2);
	return (-1);
}

int	ft_space(char c)
{
	if (c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == '\v' || c == ' ')
		return (0);
	return (-1);
}

void	ft_exit_d(t_data *data, t_params *params)
{
	write(1, "exit\n", 4);
	write(1, "\n", 1);
	free_params(params);
	free_struct(data);
	exit(params->exit_code);
}

void	replace_quotes2(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			str[i] = -1;
		else if (str[i] == '\"')
			str[i] = -2;
		i++;
	}
}

void	print_tab(char **tab)
{
	int	i;
	int	j;

	i = 0;
	while (tab[i])
	{
		j = -1;
		while (tab[i][++j])
		{
			if (tab[i][j] == -1)
				ft_printf("\'");
			else if (tab[i][j] == -2)
				ft_printf("\"");
			else
				ft_printf("%c", tab[i][j]);
		}
		if (tab[i][0] != '\0')
			ft_printf("\n");
		i++;
	}
}

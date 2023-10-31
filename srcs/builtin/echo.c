/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:55:20 by thsamina          #+#    #+#             */
/*   Updated: 2023/10/31 14:28:33 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_export(t_params *params)
{
	t_export	*tmp;
	int			i;

	tmp = params->export;
	while (params->export)
	{
		if (params->export->name[0] != '\0')
		{
			ft_printf("declare -x %s=\"", params->export->name);
			i = 0;
			while (params->export->value[i] && params->export->value[++i])
			{
				if (params->export->value[i] == -1)
					ft_printf("\'");
				else if (params->export->value[i] == -2)
					ft_printf("\"");
				else
					ft_printf("%c", params->export->value[i]);
			}
			ft_printf("\"\n");
		}
		params->export = params->export->next;
	}
	params->export = tmp;
	return (0);
}

static int	flag_n(char **arg)
{
	int	i;
	int	count;

	count = 1;
	while (arg[count])
	{
		i = 0;
		if (arg[count][i] != '-')
			return (count);
		i++;
		while (arg[count][i])
		{
			if (arg[count][i] != 'n')
				return (count);
			i++;
		}
		count++;
	}
	return (count);
}

int	ft_echo(char **arg)
{
	int	count;
	int	line;

	count = flag_n(arg);
	line = 0;
	if (count != 1)
		line++;
	while (arg[count] && arg[count + 1])
		ft_printf("%s ", arg[count++]);
	if (arg[count])
		ft_printf("%s", arg[count]);
	if (line != 1)
		ft_printf("\n");
	return (0);
}

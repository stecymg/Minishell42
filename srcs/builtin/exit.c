/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:55:49 by thsamina          #+#    #+#             */
/*   Updated: 2023/10/31 16:05:27 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_check_exit(char *nb)
{
	char	*long_long;
	int		i;

	i = 0;
	if (nb[0] == '-')
	{
		long_long = "-9223372036854775808";
		i = 1;
	}
	else
		long_long = "9223372036854775807";
	if (ft_strlen(nb) >= ft_strlen(&long_long[i]))
	{
		while (nb[i])
		{
			if (nb[i] > long_long[i])
				return (-1);
			i++;
		}
		return (0);
	}
	else
		return (0);
}

static int	parse_exit(char **arg, t_params *params, int *old_fd)
{
	int	i;

	i = -1;
	while (arg[1][++i])
	{
		if (arg[1][i] == '+' || arg[1][i] == '-')
			i++;
		if (ft_isdigit(arg[1][i]) == 0 || ft_check_exit(arg[1]) == -1)
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arg[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			params->exit_code = 2;
			free_exit(params, params->data, old_fd);
			exit(2);
		}
	}
	return (0);
}

int	ft_exit(char **arg, int i, t_params *params, int *old_fd)
{
	long int	n;

	if (i == 0)
		ft_putstr_fd("exit\n", 2);
	n = 0;
	if (arg[1])
	{
		parse_exit(arg, params, old_fd);
		n = ft_atol(arg[1]);
	}
	if (arg[1] != NULL && arg[2] != NULL)
	{
		ft_putstr_fd("minisehll: exit: too many arguments\n", 2);
		params->exit_code = 1;
		return (1);
	}
	if (g_sig && n == 0)
		params->exit_code = g_sig;
	else
		params->exit_code = n;
	free_exit(params, params->data, old_fd);
	exit(params->exit_code);
}

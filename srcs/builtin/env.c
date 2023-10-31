/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:29:56 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:20:20 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	new_env(int count, int l, t_params *params, char *arg)
{
	char	**new_env;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (new_env == NULL)
		return (set_err_malloc("export\n"));
	count = -1;
	while (params->env[++count])
		new_env[count] = params->env[count];
	new_env[count] = ft_strdup(arg);
	if (new_env[count] == NULL)
	{
		free(new_env);
		return (set_err_malloc("export\n"));
	}
	if (new_env[count][l - 2] == '+')
		ft_memmove(&new_env[count][l - 2], &new_env[count][l - 1],
			ft_strlen(&new_env[count][l - 1]) + 1);
	free(params->env);
	new_env[count + 1] = NULL;
	params->env = new_env;
	return (0);
}

static int	if_replace(char *arg, t_params *params, int count)
{
	free(params->env[count]);
	params->env[count] = ft_strdup(arg);
	if (params->env[count] == NULL)
		return (set_err_malloc("export\n"));
	return (0);
}

static int	if_concat(char *arg, t_params *params, int n)
{
	char	*tmp;

	tmp = malloc(sizeof(char) * (ft_strlen(arg)
				+ ft_strlen(params->env[n]) + 1));
	if (tmp == NULL)
		return (set_err_malloc("export\n"));
	tmp[0] = '\0';
	ft_strcat(tmp, params->env[n]);
	ft_strcat(tmp, arg);
	free(params->env[n]);
	params->env[n] = tmp;
	return (0);
}

int	ft_env(char **arg, t_params *params)
{
	if (arg[1] != NULL)
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(arg[1], 2);
		ft_putstr_fd("’: too many arguments\n", 2);
		return (1);
	}
	if (params->env == NULL)
		ft_printf("\n");
	else
		print_tab(params->env);
	return (0);
}

int	if_new_env(char *arg, t_params *params)
{
	int		count;
	int		l;

	l = 0;
	while (arg[l] != '\0' && arg[l] != '=')
		l++;
	if (arg[l] == '\0')
		return (0);
	l++;
	replace_quotes2(&arg[l]);
	count = -1;
	while (params->env[++count])
	{
		if (ft_strncmp(arg, params->env[count], l - 2) == 0
			&& params->env[count][l - 2] == '=' && arg[l - 2] == '+')
			return (if_concat(&arg[l], params, count));
		if (ft_strncmp(arg, params->env[count], l) == 0)
			return (if_replace(arg, params, count));
	}
	return (new_env(count, l, params, arg));
}

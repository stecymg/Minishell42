/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:54:44 by thsamina          #+#    #+#             */
/*   Updated: 2023/10/31 15:25:48 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_error_cd(char **arg)
{
	if (arg[1] == NULL)
	{
		ft_putstr_fd("minishell: cd: not enough arguments\n", 2);
		return (1);
	}
	if (arg[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (chdir(arg[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(arg[1], 2);
		perror(": ");
		return (1);
	}
	return (0);
}

static int	ft_if_cd(int i, int j, char **env)
{
	if (i == -1)
	{
		if (env[j] == NULL)
			return (1);
	}
	else if (j != 1)
	{
		if (env[i] == NULL)
			return (1);
	}
	else if (env[i] == NULL || env[j] == NULL)
	{
		if (env[i] == NULL)
			free(env[i]);
		if (env[j] == NULL)
			free(env[j]);
		return (1);
	}
	return (0);
}

static int	ft_end_cd(int i, int j, char **env, char *path)
{
	char	*oldpath;

	if (i != -1)
		oldpath = ft_strjoin("OLD", env[i]);
	else
		oldpath = ft_strdup("\0");
	if (oldpath == NULL)
		return (set_err_malloc("cd\n"));
	if (j != -1)
	{
		free(env[j]);
		env[j] = oldpath;
	}
	if (i != -1)
	{
		free(env[i]);
		env[i] = ft_strjoin("PWD=", path);
	}
	return (ft_if_cd(i, j, env));
}

static int	ft_find(char **env, char *str, int i)
{
	int	count;

	count = 0;
	while (env[count] && ft_strncmp(env[count], str, i))
		count++;
	if (env[count] == NULL)
		return (-1);
	return (count);
}

int	ft_cd(char **arg, t_params *params)
{
	int		i;
	int		j;
	char	path[4095];

	if (ft_error_cd(arg) == 1)
	{
		params->exit_code = 1;
		return (1);
	}
	i = ft_find(params->env, "PWD=", 4);
	j = ft_find(params->env, "OLDPWD=", 7);
	if (getcwd(path, 4095) == NULL)
	{
		perror("Minishell: chdir: error retrieving current directory: \
			getcwd: cannot access parent directories");
		return (1);
	}
	if (i == -1 && j == -1)
		return (0);
	return (ft_end_cd(i, j, params->env, path));
}

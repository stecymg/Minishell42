/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:29:25 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:20:10 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_shlvl(char **envp)
{
	char	shlvl[12];
	char	*tmp;

	while (*envp && ft_strncmp("SHLVL=", *envp, 6))
		envp++;
	if (!*envp)
		return (-1);
	ft_itoa_no_malloc(ft_atoi(*envp + 6) + 1, shlvl);
	tmp = ft_strndup(*envp, 6);
	if (tmp == NULL)
		return (set_err_malloc("creating env\n"));
	free(*envp);
	*envp = ft_strjoin(tmp, shlvl);
	free(tmp);
	if (*envp == NULL)
		return (set_err_malloc("creating env\n"));
	return (0);
}

int	init_params(t_params *params, char **envp)
{
	if (envp != NULL)
	{
		params->exit_code = 0;
		params->env = init_env(envp);
		if (errno == 12)
			return (12);
		if (params->env[0] != NULL)
			ft_shlvl(params->env);
		if (errno == 12)
			return (wrong_malloc(*params));
		params->export = init_export(params->env);
		if (errno == 12)
			return (wrong_malloc(*params));
	}
	return (0);
}

void	init_data(t_data *data)
{
	data->input = NULL;
	data->trimmed = NULL;
	data->head = NULL;
	data->pid = NULL;
	data->pipe_fd = NULL;
	data->i = 0;
}

char	**init_env(char **envp)
{
	char	**env;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	env = malloc(sizeof(char *) * (count + 1));
	if (env == NULL)
		return (err_malloc_return("creating env\n", NULL));
	count = 0;
	while (envp[count])
	{
		env[count] = ft_strdup(envp[count]);
		if (env[count] == NULL)
		{
			free_arr(env);
			return (err_malloc_return("creating env\n", NULL));
		}
		replace_quotes2(env[count]);
		count++;
	}
	env[count] = NULL;
	return (env);
}

int	wrong_malloc(t_params params)
{
	free_params(&params);
	return (errno);
}

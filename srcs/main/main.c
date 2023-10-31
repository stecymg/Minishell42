/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:56:07 by thsamina          #+#    #+#             */
/*   Updated: 2023/10/31 16:07:00 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig = 0;

//if problm with malloc
int	ft_err_malloc(t_data *data, t_params *params)
{
	free_struct(data);
	rl_clear_history();
	params->exit_code = 12;
	return (12);
}

int	minishell(t_data *data, t_params *params)
{
	t_token	*tmp;

	if (split_cmd(data, params) == -1)
		return (ft_err_malloc(data, params));
	tmp = data->head;
	if (params->error == -2)
		return (0);
	if (run_cmd(tmp, params) == -1)
		return (ft_err_malloc(data, params));
	return (0);
}

int	prompt(t_data *data, t_params *params)
{
	int		syntax;

	params->data = data;
	while (1)
	{
		params->old_fd[0] = -1;
		params->old_fd[1] = -1;
		params->error = 0;
		init_data(data);
		ft_signal(DEFAULT);
		data->input = readline("minishell: ");
		if (!data->input)
			ft_exit_d(data, params);
		syntax = syntax_check(data);
		if (syntax == 2)
			only_heredocs(data, params);
		if (syntax != -1)
			if (minishell(data, params) == 12)
				return (12);
		free_struct(data);
	}
	rl_clear_history();
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_data		data;
	t_params	params;

	(void)av;
	errno = 0;
	if (ac != 1)
		printf("Error\nNeed ./minishell\n");
	else if (isatty(1) || isatty(0) || isatty(2))
	{
		params.export = NULL;
		if (init_params(&params, envp) != 0)
			return (12);
		prompt(&data, &params);
		free_export(params.export);
		free_tab(params.env);
		return (params.exit_code);
	}
	return (0);
}

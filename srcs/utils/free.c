/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:56:07 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:17:14 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_params(t_params *params)
{
	t_export	*tmp;

	free_arr(params->env);
	while (params->export)
	{
		tmp = params->export;
		free(tmp->name);
		free(tmp->value);
		params->export = params->export->next;
		free(tmp);
	}
}

void	free_cmd(t_params *params, int *old_fd)
{
	t_token	*tmp;

	tmp = params->data->head;
	while (tmp)
	{
		if (tmp->fds[0] > 0)
			close(tmp->fds[0]);
		if (tmp->fds[1] != 1 && tmp->fds[1] >= 0)
			close(tmp->fds[1]);
		tmp = tmp->next;
	}
	free(params->data->pid);
	free(params->data->pipe_fd);
	free_struct(params->data);
	free_params(params);
	if (old_fd != NULL)
	{
		close(old_fd[0]);
		close(old_fd[1]);
	}
	exit(params->exit_code);
}

int	free_exit(t_params *params, t_data *data, int *old_fd)
{
	if (old_fd != NULL)
	{
		close(old_fd[0]);
		close(old_fd[1]);
	}
	free(data->pid);
	free(data->pipe_fd);
	free_struct(data);
	free_params(params);
	return (12);
}

void	free_struct(t_data *data)
{
	t_token	*tmp;

	if (data->input != NULL)
		free(data->input);
	if (data->trimmed != NULL)
		free(data->trimmed);
	while (data->head != NULL)
	{
		tmp = data->head;
		if (tmp->fds[0] > 0)
			close(tmp->fds[0]);
		if (tmp->fds[1] != 1 && tmp->fds[1] >= 0)
			close(tmp->fds[1]);
		free(tmp->value);
		free_arr(tmp->args);
		free_arr(tmp->red);
		data->head = data->head->next;
		free(tmp);
	}
	init_data(data);
}

void	*free_export(t_export *export)
{
	t_export	*tmp;

	while (export)
	{
		if (export->name)
			free(export->name);
		if (export->value)
			free(export->value);
		tmp = export;
		export = export->next;
		free(tmp);
	}
	return (NULL);
}

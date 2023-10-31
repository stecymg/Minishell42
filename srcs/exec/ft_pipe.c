/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:54:44 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:20:37 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_dup(t_token *token, t_pipe_fd *pipe_fd, int i)
{
	close((pipe_fd[i].raw[0]));
	if (i != 0 && token->fds[0] == 0)
	{
		dup2(pipe_fd[i - 1].raw[0], 0);
		close((pipe_fd[i - 1].raw[0]));
	}
	else if (token->fds[0] != 0)
	{
		dup2(token->fds[0], 0);
		close((token->fds[0]));
		if (i != 0)
			close((pipe_fd[i - 1].raw[0]));
	}
	if (token->fds[1] != 1)
	{
		dup2(token->fds[1], 1);
		close((token->fds[1]));
	}
	else if (token->next != NULL)
	{
		dup2(pipe_fd[i].raw[1], 1);
		close((pipe_fd[i].raw[1]));
	}
}

static void	in_child(t_token *tok, t_params *par, t_pipe_fd *pipe_fd, int i)
{
	t_token	*new;

	ft_signal(CMD);
	ft_dup(tok, pipe_fd, i);
	new = tok;
	while (new)
	{
		close((pipe_fd[i].raw[0]));
		close((pipe_fd[i].raw[1]));
		i++;
		new = new->next;
	}
	is_builtin(tok, par, 1, NULL);
}

static void	free_child(t_params *params, int *pid, t_pipe_fd *pipe_fd)
{
	t_token	*token;
	t_token	*tmp;

	token = params->data->head;
	free_params(params);
	free(pid);
	free(pipe_fd);
	free(params->data->trimmed);
	while (token != NULL)
	{
		tmp = token;
		if (tmp->fds[0] > 0)
			close(tmp->fds[0]);
		if (tmp->fds[1] != 1 && tmp->fds[1] >= 0)
			close(tmp->fds[1]);
		free(tmp->value);
		free_arr(tmp->args);
		free_arr(tmp->red);
		token = token->next;
		free(tmp);
	}
	exit(params->exit_code);
}

static void	get_exit_st(t_params *params, int i, int *pid)
{
	int	save;

	save = 0;
	while (save <= i)
	{
		if (0 < waitpid(pid[save++], &params->exit_code, 0)
			&& (WIFEXITED(params->exit_code)))
			params->exit_code = WEXITSTATUS(params->exit_code);
		else if (WIFSIGNALED(params->exit_code))
			params->exit_code = 128 + WTERMSIG(params->exit_code);
		else if (WIFSTOPPED(params->exit_code))
			params->exit_code = 128 + WSTOPSIG(params->exit_code);
	}
}

int	ft_pipe(t_token *token, t_params *params, int *pid, t_pipe_fd *pipe_fd)
{
	int	i;

	i = -1;
	while (token)
	{
		ft_signal(MUTE);
		pid[++i] = fork();
		if (check_child(params, pid[i]) == -1)
			return (-1);
		if (pid[i] == 0)
		{
			in_child(token, params, pipe_fd, i);
			free_child(params, pid, pipe_fd);
		}
		close(pipe_fd[i].raw[1]);
		if (i != 0)
			close((pipe_fd[i - 1].raw[0]));
		if (token->next == NULL)
			close((pipe_fd[i].raw[0]));
		token = token->next;
	}
	get_exit_st(params, i, pid);
	check_exit_status(params);
	ft_signal(DEFAULT);
	return (0);
}

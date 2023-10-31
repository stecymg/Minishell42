/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:33:51 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:20:49 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_exec(t_token *token, t_params *params, int *old_fd)
{
	ft_signal(CMD);
	get_path(token->args, token, params, old_fd);
	if (errno == 12)
		return (-1);
	execve(token->args[0], token->args, params->env);
	write(2, "minishell: ", 11);
	write(2, token->args[0], ft_strlen(token->args[0]));
	write(2, ": command not found\n", 20);
	params->exit_code = 127;
	free_cmd(params, old_fd);
	return (0);
}

// execve if not built-in
void	exec_cmd(t_token *token, t_params *params, int i, int *old_fd)
{
	int	pid;

	pid = 0;
	if (i == 0)
	{
		ft_signal(MUTE);
		pid = fork();
		if (check_child(params, pid) == -1)
			return ;
	}
	if (pid == 0)
		if (ft_exec(token, params, old_fd) == -1)
			return ;
	if (i == 0)
	{
		if (0 < waitpid(pid, &params->exit_code, 0)
			&& (WIFEXITED(params->exit_code)))
			params->exit_code = WEXITSTATUS(params->exit_code);
		else if (WIFSIGNALED(params->exit_code))
			params->exit_code = 128 + WTERMSIG(params->exit_code);
		check_exit_status(params);
	}
	ft_signal(DEFAULT);
}

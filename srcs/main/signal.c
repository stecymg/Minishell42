/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:28:53 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:11:29 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exit_status(t_params *params)
{
	if (g_sig == (128 | SIGQUIT))
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
	else if (g_sig == (128 | SIGSEGV))
		ft_putstr_fd("Segmentation fault (core dumped)\n", STDERR_FILENO);
	else if (g_sig == (128 | SIGINT))
		ft_putchar_fd('\n', STDERR_FILENO);
	// params->exit_code = 128;
}

void	re_prompt(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig = 130;
	}
}

void	back_slash(int sig)
{
	if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		g_sig = 131;
	}
}

void	ctrl_c_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		if (STDIN_FILENO != -1)
			close(STDIN_FILENO);
		g_sig = 130;
	}
}

void	ft_signal(t_sig_mode sig)
{
	if (sig == DEFAULT)
	{
		signal(SIGINT, re_prompt);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTERM, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
	}
	else if (sig == HEREDOC)
	{	
		signal(SIGINT, ctrl_c_heredoc);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
	}
	else if (sig == MUTE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
	}
	else if (sig == CMD)
	{
		signal(SIGINT, re_prompt);
		signal(SIGQUIT, back_slash);
		signal(SIGTSTP, SIG_IGN);
	}
}

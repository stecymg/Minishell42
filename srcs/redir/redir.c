/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:29:33 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 15:54:26 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fd_out(t_params *params, char *pathname, char *red)
{
	int	fd;

	fd = -1;
	if (ft_strcmp(red, ">") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 00664);
	else if (ft_strcmp (red, ">>") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_APPEND, 00664);
	if (check_fd(params, fd, pathname) == -1)
		return (-1);
	return (fd);
}

static void	re_out(t_params *params, char **str, int *i, t_token *token)
{
	if (str[*i][0] == '>')
	{
		if (token->fds[1] != 1 && token->fds[1] != -1)
			close(token->fds[1]);
		if (str[*i][1] == '>')
			token->fds[1] = fd_out(params, &str[*i][2], ">>");
		else
			token->fds[1] = fd_out(params, &str[*i][1], ">");
	}
}

static void	re_in(char **str, int *i, t_token *token, t_params *params)
{
	if (str[*i][0] == '<')
	{
		if (token->fds[0] > 0)
			close(token->fds[0]);
		if (str[*i][1] == '<')
		{
			if (params->error != -2)
			{
				params->exit_code = 0;
				params->old_fd[0] = dup(0);
				token->fds[0] = ft_heredoc(&str[*i][2], params);
				if (token->fds[0] == -2)
					params->error = -2;
				if (params->old_fd[0] != -1)
					close(params->old_fd[0]);
			}
		}
		else
			token->fds[0] = fd_in(params, &str[*i][1], "<");
	}
}

int	fd_in(t_params *params, char *pathname, char *red)
{
	int	fd;

	fd = 0;
	if (ft_strcmp(red, "<") == 0)
	{
		fd = open(pathname, O_RDONLY);
		if (check_fd(params, fd, pathname) == -1)
			return (-1);
	}
	return (fd);
}

void	ft_redir(char **str, t_params *params, t_token *token)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		re_out(params, str, &i, token);
		re_in(str, &i, token, params);
		if (token->fds[0] == -1 || token->fds[1] == -1)
			break ;
	}
}

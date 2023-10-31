/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:29:33 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 15:59:21 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*hd_name(void)
{
	char	*pathname;
	int		i;
	int		fd_exist;
	char	*tmp;

	i = 1;
	fd_exist = 0;
	while (1)
	{
		tmp = ft_itoa(i);
		if (tmp == NULL)
			return (err_malloc_return("heredoc\n", NULL));
		pathname = ft_strjoin("/tmp/hd", tmp);
		free(tmp);
		if (pathname == NULL)
			return (err_malloc_return("heredoc\n", NULL));
		fd_exist = open(pathname, O_RDONLY);
		if (fd_exist == -1)
			break ;
		close(fd_exist);
		++i;
		free(pathname);
	}
	return (pathname);
}

static int	verif_err(char *pathname, t_params *params, int child)
{
	if (errno == 12)
	{
		unlink(pathname);
		close(STDIN_FILENO);
		unlink(pathname);
		free(pathname);
		exit(free_exit(params, params->data, NULL));
	}
	if (check_child(params, child) == -1)
	{
		unlink(pathname);
		free(pathname);
		return (-1);
	}
	return (0);
}

static int	then_child(char *delim, int *utils, t_params *params, char *path)
{
	free(path);
	get_hd_line(delim, utils[0], utils[1], params);
	if (utils[0] != -1)
		close(utils[0]);
	free_struct(params->data);
	free_params(params);
	close(params->old_fd[0]);
	if (errno == 12)
		exit(12);
	exit(params->exit_code);
}

static int	fork_heredoc(char *delim, int *utils, t_params *params, char *path)
{
	pid_t	pid;

	pid = fork();
	if (check_child(params, pid) == -1)
	{
		unlink(path);
		free(path);
		return (-1);
	}
	if (pid == 0)
		return (then_child(delim, utils, params, path));
	if (pid != -1 && (0 < waitpid(pid, &params->exit_code, 0)))
		params->exit_code = WEXITSTATUS(params->exit_code);
	if (WIFSIGNALED(params->exit_code) && WTERMSIG(params->exit_code))
	{
		unlink(path);
		free(path);
		close(utils[0]);
		params->exit_code = 130;
		return (1);
	}
	return (0);
}

int	ft_heredoc(char *delim, t_params *params)
{
	char		*pathname;
	int			i[2];
	int			child;

	i[1] = delim_quotes(delim);
	pathname = hd_name();
	if (pathname == NULL)
	{
		close(STDIN_FILENO);
		exit(free_exit(params, params->data, NULL));
	}
	i[0] = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 00664);
	ft_signal(MUTE);
	child = fork_heredoc(delim, i, params, pathname);
	if (child == 1)
		return (-2);
	if (verif_err(pathname, params, child) == -1)
		return (-1);
	ft_signal(DEFAULT);
	if (i[0] != -1)
		close(i[0]);
	i[0] = fd_in(params, pathname, "<");
	unlink(pathname);
	free(pathname);
	return (i[0]);
}

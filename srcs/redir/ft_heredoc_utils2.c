/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 16:19:38 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:19:50 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	end_new(char *new, int fd)
{
	replace_quotes(new);
	ft_putstr_fd(new, fd);
	ft_putstr_fd("\n", fd);
	free(new);
}

static void	check_signals(char *line, char *del, int fd, t_params *params)
{
	if (!line && g_sig != 130)
		print_error_heredoc(del, fd, params);
	if (!line && g_sig == 130)
	{
		params->exit_code = 130;
		free_heredoc(params, fd);
		exit(130);
	}
}

static char	*line_to_write(char *line, int quotes, t_params *params)
{
	int		i;
	char	*new;

	i = 0;
	new = NULL;
	while (line[i])
	{
		if (line[i] == '$' && quotes == 0)
			line = expand_heredoc(line, params, &i);
		if (line == NULL)
			return (NULL);
		i++;
	}
	new = ft_strdup(line);
	free(line);
	return (new);
}

int	get_hd_line(char *del, int fd, int quotes, t_params *params)
{
	char	*line;
	char	*new;
	char	*tmp;

	new = NULL;
	while (1)
	{
		ft_signal(HEREDOC);
		line = readline("> ");
		check_signals(line, del, fd, params);
		tmp = ft_strdup(line);
		if (tmp == NULL)
			return (set_err_malloc("heredoc\n"));
		if (line && ft_strcmp(line, del) == 0)
		{
			free(tmp);
			break ;
		}
		if (line)
			new = line_to_write(tmp, quotes, params);
		if (new == NULL)
			return (-1);
		end_new(new, fd);
	}
	return (0);
}

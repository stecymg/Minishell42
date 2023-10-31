/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:29:38 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 16:19:14 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_var_heredoc(char *str, int first, char *line, int *i)
{
	char	*tmp;

	if (str == NULL)
	{
		ft_memmove(&line[first], &line[i[0]], ft_strlen(&line[i[0]]) + 1);
		i[0] = first - 1;
	}
	else
	{
		tmp = malloc(sizeof(char) * (first + ft_strlen(str)
					+ ft_strlen(&line[i[0]]) + 1));
		if (tmp == NULL)
			return (err_malloc_return("heredoc\n", NULL));
		ft_bzero(tmp, first + ft_strlen(str)
			+ ft_strlen(&line[i[0]]) + 1);
		tmp = ft_memmove(tmp, line, first);
		tmp = ft_strcat(tmp, str);
		tmp = ft_strcat(tmp, &line[i[0]]);
		free(line);
		line = tmp;
		i[0] = first + ft_strlen(str) - 1;
	}
	return (line);
}

static char	*expand_env_in_heredoc(char *str, t_params *params,
			int size, char *buff)
{
	int		count;

	if (size == 0)
		return ("$");
	if (str[0] == '?' && (ft_space(str[0] == 0) || str[0] == '\0'))
	{
		ft_itoa_no_malloc(params->exit_code, buff);
		return (buff);
	}
	count = -1;
	while (params->env[++count])
	{
		if (ft_strncmp(params->env[count], str, size) == 0
			&& params->env[count][size] == '=')
			return (&params->env[count][size + 1]);
	}
	return (NULL);
}

char	*expand_heredoc(char *line, t_params *params, int *i)
{
	int		count;
	int		first;
	char	*tmp;
	char	*tmp2;
	char	buff[12];

	count = 0;
	first = i[0];
	tmp = NULL;
	i[0]++;
	while (line[i[0]] != '$' && line[i[0]] != '\0' && line[i[0]] != '\''
		&& line[i[0]] != '\"' && (ft_space(line[i[0]]) != 0))
	{
		count++;
		i[0]++;
		if (line[i[0] - 1] == '?')
			break ;
	}
	tmp = expand_env_in_heredoc(&line[first + 1], params, count, buff);
	tmp2 = replace_var_heredoc(tmp, first, line, i);
	return (tmp2);
}

void	print_error_heredoc(char *str, int fd, t_params *params)
{
	ft_putstr_fd("minishell: warning: here-document at line delimited "
		"by end-of-file (wanted '", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("')\n", 2);
	free_heredoc(params, fd);
	params->exit_code = g_sig;
	exit(0);
}

void	free_heredoc(t_params *params, int fd)
{
	close(fd);
	dup2(params->old_fd[0], 0);
	close(params->old_fd[0]);
	free_struct(params->data);
	free_params(params);
}

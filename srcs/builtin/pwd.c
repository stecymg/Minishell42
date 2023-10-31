/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:56:24 by thsamina          #+#    #+#             */
/*   Updated: 2023/10/30 03:59:43 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **arg)
{
	char	*path;

	(void)arg;
	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("Minishell: pwd: error retrieving current directory: getcwd: \
		cannot access parent directories");
		return (1);
	}
	else
	{
		ft_printf("%s\n", path);
		free(path);
	}
	return (0);
}

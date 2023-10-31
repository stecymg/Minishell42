/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 09:59:34 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 15:52:59 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_fd(t_params *params, int fd, char *red)
{
	if (fd == -1)
	{
		ft_printf("Minishell: %s", red);
		perror(" ");
		params->exit_code = 1;
		return (-1);
	}
	return (0);
}

int	set_err_malloc(char *str)
{
	errno = ENOMEM;
	ft_putstr_fd("Out of memory in ", 2);
	ft_putstr_fd(str, 2);
	return (-1);
}

void	*err_malloc_return(char *str, void *ptr)
{
	set_err_malloc(str);
	return (ptr);
}

int	check_child(t_params *params, int pid)
{
	if (pid == -2)
		return (-1);
	if (pid < 0)
	{
		perror("Fork");
		params->exit_code = 127;
		return (-1);
	}
	return (0);
}

void	free_arr(char **tab)
{
	int	i;

	i = 0;
	if (tab == NULL)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

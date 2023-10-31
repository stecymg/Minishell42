/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   if_doll.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 14:29:12 by bfiguet           #+#    #+#             */
/*   Updated: 2023/10/31 15:36:16 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// find variable to replace $
static char	*find_var(t_params *params, char *str, int *quote, char *buff)
{
	int		count;
	char	**env;

	env = params->env;
	if ((quote[1] == 0 && (str[0] != '\"' && str[0] != '\''))
		|| (quote[2] % 2 != 0 && quote[1] == 0))
		return ("$");
	if (str[0] == '?')
	{
		ft_itoa_no_malloc(params->exit_code, buff);
		return (buff);
	}
	count = -1;
	while (env[++count])
	{
		if (ft_strncmp(env[count], str, quote[1]) == 0
			&& env[count][quote[1]] == '=')
			return (&env[count][quote[1] + 1]);
	}
	return (NULL);
}

// replace $
static int	replace_var(char *str, int s, char **str_value, t_data *data)
{
	char	*tmp;

	if (str == NULL)
	{
		ft_memmove(&str_value[0][s], &str_value[0][data->i],
			ft_strlen(&str_value[0][data->i]) + 1);
		data->i = s - 1;
	}
	else
	{
		tmp = malloc(sizeof(char) * (s + ft_strlen(str)
					+ ft_strlen(&str_value[0][data->i]) + 1));
		if (tmp == NULL)
			return (set_err_malloc("expand\n"));
		ft_bzero(tmp, s + ft_strlen(str)
			+ ft_strlen(&str_value[0][data->i]) + 1);
		tmp = ft_memmove(tmp, str_value[0], s);
		tmp = ft_strcat(tmp, str);
		tmp = ft_strcat(tmp, &str_value[0][data->i]);
		free(str_value[0]);
		str_value[0] = tmp;
		data->i = s + ft_strlen(str) - 1;
	}
	return (0);
}

int	if_dollar(char **str_value, t_data *data, t_params *par, int *quote)
{
	int		s;
	char	*str;
	char	buff[12];

	s = data->i++;
	while (str_value[0][data->i] && ft_space(str_value[0][data->i])
		!= 0 && str_value[0][data->i] != '\'' && str_value[0][
			data->i] != '\"' && str_value[0][data->i] != '$')
	{
		data->i++;
		if (str_value[0][data->i - 1] == '?')
			break ;
	}
	quote[1] = data->i - s - 1;
	if (quote[0] % 2 == 0)
	{
		str = find_var(par, &str_value[0][s + 1], quote, buff);
		if (replace_var(str, s, &str_value[0], data) == -1)
			return (-1);
	}
	return (0);
}

//find $
int	expand(t_token *token, t_data *data, t_params *params)
{
	int		quote[3];

	while (token)
	{
		quote[0] = 0;
		quote[2] = 0;
		data->i = -1;
		while (token->value[++data->i])
		{
			if (token->value[data->i] == '\"')
				quote[2]++;
			if (token->value[data->i] == '\'' && quote[2] % 2 == 0)
				quote[0]++;
			if (token->value[data->i] == '\'' && quote[0] % 2 != 0)
			{
				jump_quotes(token->value, data);
				quote[0]++;
			}
			if (token->value[data->i] == '$')
				if (if_dollar(&token->value, data, params, quote) == -1)
					return (-1);
		}
		token = token->next;
	}
	return (0);
}

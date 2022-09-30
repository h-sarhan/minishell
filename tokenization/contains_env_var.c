/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contains_env_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 08:58:03 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/30 08:35:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_env_var(const char *str, size_t *i)
{
	if (str[*i] == '\0')
		return (0);
	else if (str[*i] == '?')
		return (1);
	else if ((str[*i] == '\"' || str[*i] == '\''))
	{
		if (str[*i + 1] == '\0')
			return (0);
		else if (!ft_isalnum(str[*i + 1]) && str[*i + 1] != '_')
			*i += 1;
		else
			return (1);
	}
	else if (!ft_isalnum(str[*i]) && str[*i] != '_')
		*i += 1;
	else
		return (1);
	if (str[*i] == '\0')
		return (0);
	return (-1);
}

bool	contains_env_var(const char *str)
{
	size_t	i;
	bool	in_s_quotes;
	bool	in_d_quotes;
	int		ret;

	i = 0;
	in_s_quotes = false;
	in_d_quotes = false;
	while (str[i] != '\0')
	{
		set_in_quotes(str, i, &in_s_quotes, &in_d_quotes);
		if (str[i] == '$' && in_s_quotes == false)
		{
			i++;
			ret = is_env_var(str, &i);
			if (ret == 0)
				return (false);
			if (ret == 1)
				return (true);
			i++;
		}
		else
			i++;
	}
	return (false);
}

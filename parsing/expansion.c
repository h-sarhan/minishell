/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 21:30:28 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/17 21:40:42 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// TODO: Check for memory leaks and protect all memory allocating functions
char	*expand_env_var(char *str)
{
	size_t	i;
	size_t	start;
	size_t	end;
	char	*before;
	char	*env_var;
	char	*expansion;
	char	*after;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
		{
			i++;
			start = i;
			while (str[i] != '\0' && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			end = i - 1;
			if (start < end)
				break ;
		}
		i++;
	}
	before = ft_substr(str, 0, start - 1);
	env_var = ft_substr(str, start, end - start + 1);
	expansion = getenv(env_var);
	ft_free(&env_var);
	after = ft_substr(str, end + 1, ft_strlen(str));
	ft_free(&str);
	if (expansion == NULL)
		str = strjoin_free(before, "", 1);
	else
		str = strjoin_free(before, expansion, 1);
	str = strjoin_free(str, after, 3);
	return (str);
}

bool	contains_env_var(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
		{
			i++;
			if (str[i] == '\0')
				return (false);
			else if (!ft_isalnum(str[i]) && str[i] != '_')
				i++;
			else
				return (true);
		}
		else
			i++;
	}
	return (false);
}

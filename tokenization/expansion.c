/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 21:30:28 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/29 15:11:32 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// TODO: Protect substr and strjoin
static char	*create_env_var_str(char *str, const size_t start, const size_t end)
{
	char	*before;
	char	*env_var;
	char	*expansion;
	char	*after;
	
	// printf("%s\n", str);
	before = ft_substr(str, 0, start - 1);
	env_var = ft_substr(str, start, end - start + 1);
	expansion = getenv(env_var);
	ft_free(&env_var);
	after = ft_substr(str, end + 1, ft_strlen(str));
	ft_free(&str);
	// if (before == NULL || env_var == NULL || after == NULL)
	// 	return (str);
	if (expansion == NULL)
		str = strjoin_free(before, "", 1);
	else
		str = strjoin_free(before, expansion, 1);
	str = strjoin_free(str, after, 3);
	return (str);
}

// TODO: Expand LAST_EXIT command
char	*expand_double_quote(char *str)
{
	size_t	i;
	size_t	start;
	size_t	end;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '$' && str[i + 1] != '?')
		{
			i++;
			start = i;
			while (str[i] != '\0' && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			end = i - 1;
			if (start < end)
				break ;
		}
		else if (str[i] == '$' && str[i + 1] == '?')
		{
		}
		else
			i++;
	}
	return (create_env_var_str(str, start, end));
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

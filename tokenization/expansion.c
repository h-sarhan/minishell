/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 21:30:28 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/31 18:05:54 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// TODO: Protect substr and strjoin
static char	*create_env_var_str(const t_shell *shell, char *str, const size_t start, const size_t end)
{
	char	*before;
	char	*env_var;
	char	*expansion;
	char	*after;
	
	// printf("%s\n", str);
	before = ft_substr(str, 0, start - 1);
	env_var = ft_substr(str, start, end - start + 1);
	expansion = get_env(shell, env_var);
	ft_free(&env_var);
	after = ft_substr(str, end + 1, ft_strlen(str));
	ft_free(&str);
	// if (before == NULL || env_var == NULL || after == NULL)
	// 	return (str);
	if (expansion == NULL)
		str = strjoin_free(before, "", 1);
	else
		str = strjoin_free(before, expansion, 3);
	str = strjoin_free(str, after, 3);
	return (str);
}

// TODO: Expand LAST_EXIT command
char	*expand_double_quote(const t_shell *shell, char *str)
{
	size_t	i;
	size_t	start;
	size_t	end;
	bool	in_s_quotes;
	bool	in_d_quotes;

	i = 0;
	in_s_quotes = false;
	in_d_quotes = false;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' && in_d_quotes == false)
			in_s_quotes = !in_s_quotes;
		if (str[i] == '\"' && in_s_quotes == false)
			in_d_quotes = !in_d_quotes;
		if (str[i] == '$' && str[i + 1] != '?' && in_s_quotes == false)
		{
			i++;
			start = i;
			while (str[i] != '\0' && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			end = i - 1;
			if (start < end)
				break ;
		}
		else if (str[i] == '$' && str[i + 1] == '?' && in_s_quotes == false)
		{
		}
		else
			i++;
	}
	return (create_env_var_str(shell, str, start, end));
}

bool	contains_env_var(const char *str)
{
	size_t	i;
	bool	in_s_quotes;
	bool	in_d_quotes;

	i = 0;
	in_s_quotes = false;
	in_d_quotes = false;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' && in_d_quotes == false)
			in_s_quotes = !in_s_quotes;
		if (str[i] == '\"' && in_s_quotes == false)
			in_d_quotes = !in_d_quotes;
		if (str[i] == '$' && in_s_quotes == false)
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

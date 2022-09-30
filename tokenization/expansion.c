/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 21:30:28 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_env_var_str(const t_shell *shell, char *str,
	const size_t start, const size_t end)
{
	char	*before;
	char	*env_var;
	char	*expansion;
	char	*after;

	before = ft_substr(str, 0, start - 1);
	env_var = ft_substr(str, start, end - start + 1);
	expansion = get_env(shell, env_var);
	ft_free(&env_var);
	after = ft_substr(str, end + 1, ft_strlen(str));
	ft_free(&str);
	if (expansion == NULL)
		str = strjoin_free(before, "", 1);
	else
		str = strjoin_free(before, expansion, 3);
	str = strjoin_free(str, after, 3);
	return (str);
}

void	set_in_quotes(const char *str, const size_t i, bool *in_single,
	bool *in_double)
{
	if (str[i] == '\'' && *in_double == false)
		*in_single = !(*in_single);
	if (str[i] == '\"' && *in_single == false)
		*in_double = !(*in_double);
}

static bool	skip_to_end(const char *str, size_t *i, size_t *start, size_t *end)
{
	*i += 1;
	*start = *i;
	while (str[*i] != '\0' && (ft_isalnum(str[*i]) || str[*i] == '_'))
		*i += 1;
	*end = *i - 1;
	if (start < end)
		return (false);
	return (true);
}

char	*expand_env_var(const t_shell *shell, char *str)
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
		set_in_quotes(str, i, &in_s_quotes, &in_d_quotes);
		if (str[i] == '$' && str[i + 1] != '?' && in_s_quotes == false)
		{
			if (ft_isdigit(str[i + 1]))
				return (create_env_var_str(shell, str, i + 1, i + 1));
			if (skip_to_end(str, &i, &start, &end) == false)
				break ;
		}
		else if (str[i] == '$' && str[i + 1] == '?' && in_s_quotes == false)
			return (create_env_var_str(shell, str, i + 1, i + 1));
		else
			i++;
	}
	return (create_env_var_str(shell, str, start, end));
}

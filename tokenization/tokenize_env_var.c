/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_env_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 12:36:39 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/26 12:45:06 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_quotes(const char ch, bool *in_quote, char *quote)
{
	if (ch == '\'' || ch == '\"')
	{
		if (*quote == '\0')
		{
			*quote = ch;
			*in_quote = !(*in_quote);
		}
		else if (*quote == ch)
		{
			*in_quote = !(*in_quote);
			*quote = '\0';
		}
	}
}

static char	*get_env_string(const char *line, size_t *idx)
{
	char	*str;
	size_t	i;
	bool	in_quote;
	char	quote;

	i = *idx;
	in_quote = false;
	quote = '\0';
	while (line[i] != '\0')
	{
		set_quotes(line[i], &in_quote, &quote);
		if ((line[i] == ' ' || ft_strchr("<>|(&)", line[i])) && !in_quote)
			break ;
		i++;
	}
	if (in_quote == true)
	{
		*idx = i;
		return (NULL);
	}
	str = ft_substr(line, *idx, i - *idx);
	*idx = i - 1;
	return (str);
}

t_list	*tokenize_env_variable(const t_shell *shell, const char *line,
	size_t *idx)
{
	t_token	*tkn;
	t_list	*el;

	tkn = ft_calloc(1, sizeof(t_token));
	if (tkn == NULL)
		return (NULL);
	tkn->type = ENV_VAR;
	tkn->substr = get_env_string(line, idx);
	if (tkn->substr == NULL)
		return (NULL);
	if (ft_strncmp(tkn->substr, "$\"\"", ft_strlen(tkn->substr)) == 0
		&& ft_strlen(tkn->substr) == 3)
	{
		tkn->type = NORMAL;
		el = ft_lstnew(tkn);
		return (el);
	}
	while (contains_env_var(tkn->substr))
		tkn->substr = expand_env_var(shell, tkn->substr);
	tkn->type = NORMAL;
	tkn->substr = eat_dollars(tkn->substr);
	tkn->substr = eat_quotes(tkn->substr);
	el = ft_lstnew(tkn);
	return (el);
}

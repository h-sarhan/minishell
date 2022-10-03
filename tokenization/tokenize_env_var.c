/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_env_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 12:36:39 by hsarhan           #+#    #+#             */
/*   Updated: 2022/10/03 22:39:12 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	retokenize_env_var(t_token *token, t_list **el, t_list **tokens)
{
	char	*substr_copy;
	bool	success;

	substr_copy = ft_strdup(token->substr);
	ft_lstclear(el, free_token);
	*el = tokenize_env_var_str(substr_copy, &success);
	ft_free(&substr_copy);
	ft_lstadd_back(tokens, *el);
	return (success);
}

void	set_quotes(const char ch, char *quote, bool *in_quote)
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
		set_quotes(line[i], &quote, &in_quote);
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

	tkn = ft_calloc(1, sizeof(t_token));
	if (tkn == NULL)
		return (NULL);
	tkn->type = ENV_VAR;
	tkn->substr = get_env_string(line, idx);
	if (tkn->substr == NULL)
	{
		ft_free(&tkn);
		return (NULL);
	}
	if (ft_strncmp(tkn->substr, "$\"\"", ft_strlen(tkn->substr)) == 0
		&& ft_strlen(tkn->substr) == 3)
	{
		tkn->type = NORMAL;
		return (ft_lstnew(tkn));
	}
	tkn->substr = expand_env_var(shell, tkn->substr);
	tkn->type = NORMAL;
	return (ft_lstnew(tkn));
}

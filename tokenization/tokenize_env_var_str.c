/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_env_var_str.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 20:53:36 by hsarhan           #+#    #+#             */
/*   Updated: 2022/10/03 21:24:12 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_other(const char *line, const size_t i)
{
	return (ft_strchr("\\;`&(<>)", line[i]) != NULL);
}

static	t_list	*tokenize_other(const char *line,
	size_t *idx)
{
	size_t	i;
	t_token	*tkn;
	t_list	*el;

	i = *idx;
	tkn = ft_calloc(1, sizeof(t_token));
	if (tkn == NULL)
		return (NULL);
	tkn->start = i;
	tkn->type = NORMAL;
	while (is_other(line, i) && line[i] != '\0')
		i++;
	tkn->end = i - 1;
	tkn->substr = ft_substr(line, tkn->start, tkn->end - tkn->start + 1);
	el = ft_lstnew(tkn);
	*idx = tkn->end;
	return (el);
}

t_list		*tokenize_env_var_str(const t_shell *shell, const char *line,
	bool *success)
{
	size_t	i;
	t_list	*tokens;

	*success = true;
	tokens = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		if (is_other(line, i) == true)
			ft_lstadd_back(&tokens, tokenize_other(line, &i));
		else if (line[i] == '\'' || line[i] == '\"' || line[i] == '(')
			*success = first_token_group(shell, line, &i, &tokens);
		else if ((line[i] == '$' && last_token_was_heredoc(tokens) == false)
			|| line[i] == ')')
			*success = second_token_group(shell, line, &i, &tokens);
		else if (line[i] != ' ')
			*success = tokenize_normal_and_wildcard(shell, line, &i, &tokens);
		if (*success == false)
			return (NULL);
		if (line[i] != '\0')
			i++;
	}
	return (tokens);
}

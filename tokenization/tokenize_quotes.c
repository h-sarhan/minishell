/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 16:25:19 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/26 11:44:21 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	*parse_error(const char *msg, t_token *tkn)
{
	ft_free(&tkn);
	if (msg != NULL)
		write_to_stderr(msg);
	return (NULL);
}

static void	*skip_to_end_token(const char *line, size_t *i, t_token *tkn)
{
	char	quote;

	while (line[*i] != ' ' && line[*i] != '\0')
	{
		if (line[*i] == '\'' || line[*i] == '\"')
		{
			quote = line[*i];
			*i += 1;
			while (line[*i] != quote && line[*i] != '\0')
				*i += 1;
			if (line[*i] == '\0')
				return (parse_error("Parse Error: Unterminated string\n", tkn));
			*i += 1;
		}
		else if (ft_strchr("<>|(&)", line[*i]) != NULL)
			break ;
		else
			*i += 1;
	}
	return (tkn);
}

static t_list	*create_token_el(const t_shell *shell, t_token *tkn,
	const size_t i, const char *line)
{
	t_list	*el;

	tkn->end = i - 1;
	if (tkn->end <= tkn->start)
		tkn->substr = ft_strdup("");
	else
		tkn->substr = ft_substr(line, tkn->start, tkn->end - tkn->start + 1);
	if (tkn->substr == NULL)
		return (parse_error(NULL, tkn));
	while (contains_env_var(tkn->substr))
		tkn->substr = expand_env_var(shell, tkn->substr);
	tkn->substr = eat_quotes(tkn->substr);
	if (tkn->substr == NULL)
		return (parse_error("Parse error: Invalid Input\n", tkn));
	if (ft_strchr(tkn->substr, '*') != NULL)
		tkn->substr = expand_wildcard(tkn->substr);
	el = ft_lstnew(tkn);
	return (el);
}

t_list	*tokenize_single_quote(const t_shell *shell, const char *line,
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
	tkn->type = QUOTED_STRING;
	i++;
	while (line[i] != '\0' && line[i] != '\'')
		i++;
	if (line[i] == '\0')
		return (parse_error("Parse Error: Unterminated string\n", tkn));
	i++;
	if (skip_to_end_token(line, &i, tkn) == NULL)
		return (NULL);
	el = create_token_el(shell, tkn, i, line);
	if (el == NULL)
		return (NULL);
	*idx = tkn->end;
	return (el);
}

t_list	*tokenize_double_quote(const t_shell *shell, const char *line,
	size_t *idx)
{
	size_t	i;
	t_token	*tkn;
	t_list	*el;

	i = *idx + 1;
	tkn = ft_calloc(1, sizeof(t_token));
	if (tkn == NULL)
		return (NULL);
	tkn->start = i - 1;
	tkn->type = DOUBLE_QUOTED_STRING;
	while (line[i] != '\0' && line[i] != '\"')
		i++;
	if (line[i] == '\0')
		return (parse_error("Parse Error: Unterminated string\n", tkn));
	i++;
	if (skip_to_end_token(line, &i, tkn) == NULL)
		return (NULL);
	el = create_token_el(shell, tkn, i, line);
	if (el == NULL)
		return (NULL);
	*idx = tkn->end;
	return (el);
}

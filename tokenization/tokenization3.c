/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 16:25:19 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/01 17:44:20 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	*parse_error(const char *msg)
{
	write_to_stderr(msg);
	return (NULL);
}

t_list	*tokenize_single_quote(const t_shell *shell, const char *line, size_t *idx)
{
	size_t	i;
	t_token	*tkn;
	t_list	*el;
	char	quote;

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
		return (parse_error(("Parse Error: Unterminated string\n")));
	i++;
	while (line[i] != ' ' && line[i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote = line[i];
			while (line[i] != quote && line[i] != '\0')
				i++;
			if (line[i] == '\0')
			{
				free_token(tkn);
				return (parse_error(("Parse Error: Unterminated string\n")));
			}
		}
		i++;
	}
	tkn->end = i - 1;
	tkn->substr = ft_substr(line, tkn->start, tkn->end - tkn->start + 1);
	tkn->sub_tokens = NULL;
	if (tkn->substr == NULL)
		return (NULL);
	printf("token is %s\n", tkn->substr);
	while (contains_env_var(tkn->substr))
		tkn->substr = expand_double_quote(shell, tkn->substr);
	tkn->substr = eat_quotes(tkn->substr);
	if (tkn->substr == NULL)
	{
		free_token(tkn);
		return (parse_error("Parse error: Invalid Input\n"));
	}
	el = ft_lstnew(tkn);
	if (el == NULL)
		return (NULL);
	*idx = i;
	return (el);
}

t_list	*tokenize_double_quote(const t_shell *shell, const char *line, size_t *idx)
{
	size_t	i;
	t_token	*tkn;
	t_list	*el;
	char	quote;

	i = *idx + 1;
	tkn = ft_calloc(1, sizeof(t_token));
	if (tkn == NULL)
		return (NULL);
	tkn->start = i - 1;
	tkn->type = DOUBLE_QUOTED_STRING;
	while (line[i] != '\0' && line[i] != '\"')
		i++;
	if (line[i] == '\0')
		return (parse_error(("Parse Error: Unterminated string\n")));
	i++;
	while (line[i] != ' ' && line[i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote = line[i];
			while (line[i] != quote && line[i] != '\0')
				i++;
			if (line[i] == '\0')
			{
				free_token(tkn);
				return (parse_error(("Parse Error: Unterminated string\n")));
			}
		}
		i++;
	}
	tkn->end = i - 1;
	if (tkn->end <= tkn->start)
		tkn->substr = ft_strdup("");
	else
		tkn->substr = ft_substr(line, tkn->start, tkn->end - tkn->start + 1);
	if (tkn->substr == NULL)
	{
		free_token(tkn);
		return (NULL);
	}
	while (contains_env_var(tkn->substr))
		tkn->substr = expand_double_quote(shell, tkn->substr);
	tkn->substr = eat_quotes(tkn->substr);
	if (tkn->substr == NULL)
	{
		free_token(tkn);
		return (parse_error("Parse error: Invalid Input\n"));
	}
	el = ft_lstnew(tkn);
	if (el == NULL)
		return (NULL);
	*idx = i;
	return (el);
}

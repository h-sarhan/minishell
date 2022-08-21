/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 22:19:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 12:03:55 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	*parse_error(const char *msg)
{
	write_to_stderr(msg);
	return (NULL);
}

t_list	*tokenize_normal(const char *line, size_t *idx)
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
	while (line[i] != '\0' && ft_strchr(" \'\"$<>|(", line[i]) == NULL)
		i++;
	tkn->end = i - 1;
	tkn->substr = ft_substr(line, tkn->start, tkn->end - tkn->start + 1);
	if (tkn->substr == NULL)
		return (NULL);
	if (ft_strchr(tkn->substr, '*') != NULL)
	{
		tkn->type = WILDCARD;
		tkn->substr = expand_wildcard(tkn->substr);
	}
	el = ft_lstnew(tkn);
	*idx = tkn->end;
	return (el);
}

static t_list	*tokenize_subexpr_helper(t_token *tkn, const size_t i,
					const char *line, size_t *idx)
{
	bool	success;
	t_list	*el;

	tkn->end = i - 1;
	tkn->substr = ft_substr(line, tkn->start + 1, tkn->end - tkn->start - 1);
	if (tkn->substr == NULL)
		return (NULL);
	success = true;
	tkn->sub_tokens = tokenize_line(tkn->substr, &success);
	if (success == false)
		return (NULL);
	el = ft_lstnew(tkn);
	*idx = tkn->end;
	return (el);
}

t_list	*tokenize_subexpr(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*token;
	int		paren_counter;

	i = *idx + 1;
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = i - 1;
	token->type = SUB_EXPR;
	paren_counter = 1;
	while (line[i] != '\0' && paren_counter != 0)
	{
		if (line[i] == '(')
			paren_counter++;
		else if (line[i] == ')')
			paren_counter--;
		i++;
	}
	if (paren_counter != 0)
		return (parse_error("Parse Error: Invalid input\n"));
	return (tokenize_subexpr_helper(token, i, line, idx));
}

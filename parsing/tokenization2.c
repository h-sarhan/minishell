/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 22:19:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/19 09:35:48 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_list	*tokenize_operator(const char *line, size_t *idx, const t_token_type type)
{
	size_t	i;
	t_token	*token;
	t_list	*el;

	i = *idx;
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = i;
	token->type = type;
	token->end = i;
	if (type == HEREDOC || type == APPEND_REDIR || type == LAST_EXIT || type == AND || type == OR)
	{
		token->end++;
		token->substr = ft_substr(line, token->start, 2);
	}
	else
		token->substr = ft_substr(line, token->start, 1);
	if (token->substr == NULL)
		return (NULL);
	token->sub_tokens = NULL;
	el = ft_lstnew(token);
	if (el == NULL)
		return (NULL);
	*idx = i;
	if (type == HEREDOC || type == APPEND_REDIR || type == LAST_EXIT || type == AND || type == OR)
		*idx = i + 1;
	return (el);
}

t_list	*tokenize_normal(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*token;
	t_list	*el;

	i = *idx;
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = i;
	token->type = NORMAL;
	// TODO: Replace with a is_whitespace function
	while (line[i] != '\0' && ft_strchr(" \'\"$<>|(", line[i]) == NULL)
		i++;
	token->end = i - 1;	
	token->substr = ft_substr(line, token->start, token->end - token->start + 1);
	if (token->substr == NULL)
		return (NULL);
	token->sub_tokens = NULL;
	if (ft_strchr(token->substr, '*') != NULL)
	{
		token->type = WILDCARD;
		// ft_free(&token->substr);
		token->substr = expand_wildcard(token->substr);
	}
	el = ft_lstnew(token);
	if (el == NULL)
		return (NULL);
	*idx = token->end;
	return (el);
}

t_list	*tokenize_subexpr(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*token;
	t_list	*el;
	bool	success;
	int		paren_counter;

	i = *idx;
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = i;
	token->type = SUB_EXPR;
	// TODO: Replace with a is_whitespace function
	paren_counter = 1;
	// printf("Before parsing subexpr: %s\n", line);
	i++;
	while (line[i] != '\0' && paren_counter != 0)
	{
		if (line[i] == '(')
			paren_counter++;
		else if (line[i] == ')')
			paren_counter--;
		i++;
	}
	if (paren_counter != 0)
	{
		write_to_stderr("Parse Error: Invalid input\n");
		return (NULL);
	}
	token->end = i - 1;	
	token->substr = ft_substr(line, token->start + 1, token->end - token->start - 1);
	if (token->substr == NULL)
		return (NULL);
	success = true;
	token->sub_tokens = tokenize_line(token->substr, &success);
	// printf("(%s) sub tokens:\n", token->substr);
	// print_tokens(token->sub_tokens);
	// token->sub_tokens = NULL;
	
	if (success == false)
		return (NULL);
	el = ft_lstnew(token);
	if (el == NULL)
		return (NULL);
	*idx = token->end;
	return (el);
}

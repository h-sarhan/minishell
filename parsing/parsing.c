/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:46:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/17 18:59:15 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_list	*tokenize_single_quote(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*token;
	t_list	*el;

	i = *idx;
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = i;
	token->type = QUOTED_STRING;
	token->expanded = true;
	i++;
	while (line[i] != '\0' && line[i] != '\'')
		i++;
	if (line[i] == '\0')
	{
		write_to_stderr("Parse Error: Unterminated string\n");
		return (NULL);
	}
	else
		token->end = i;
	el = ft_lstnew(token);
	if (el == NULL)
		return (NULL);
	*idx = i;
	return (el);
}

static t_list	*tokenize_double_quote(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*token;
	t_list	*el;

	i = *idx;
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = i;
	token->type = DOUBLE_QUOTED_STRING;
	token->expanded = false;
	i++;
	while (line[i] != '\0' && line[i] != '\"')
		i++;
	if (line[i] == '\0')
	{
		write_to_stderr("Parse Error: Unterminated string\n");
		return (NULL);
	}
	else
		token->end = i;
	el = ft_lstnew(token);
	if (el == NULL)
		return (NULL);
	*idx = i;
	return (el);
}

// TODO: Free in case of malloc errors
t_list	*parse_line(const char *line, bool *success)
{
	size_t	i;
	t_list	*tokens;
	t_list	*el;

	// * Scanning for invalid operators
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\\' || line[i] == ';')
		{
			write_to_stderr("Parse Error: Invalid operator\n");
			*success = false;
			return (NULL);
		}
		i++;
	}
	tokens = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'')
		{
			el = tokenize_single_quote(line, &i);
			if (el == NULL)
			{
				// ? ft_lstclear here maybe??
				*success = false;
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		if (line[i] == '\"')
		{
			el = tokenize_double_quote(line, &i);
			if (el == NULL)
			{
				// ? ft_lstclear here maybe??
				*success = false;
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		i++;
	}
	return (tokens);
}

void	print_tokens(const char *line, t_list *tokens)
{
	t_token	*token;
	char	*substr;

	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->type == QUOTED_STRING)
		{
			substr = ft_substr(line, token->start, token->end - token->start + 1);
			if (substr == NULL)
			{
				exit(EXIT_FAILURE);
			}
			printf("Quoted string: %s\nstart=(%zu)\nend=(%zu)\n", substr, token->start,
					token->end);
		}
		if (token->type == DOUBLE_QUOTED_STRING)
		{

			substr = ft_substr(line, token->start, token->end - token->start + 1);
			if (substr == NULL)
			{
				exit(EXIT_FAILURE);
			}
			printf("Double quoted string: %s\nstart=(%zu)\nend=(%zu)\n", substr,
					token->start, token->end);
		}
		tokens = tokens->next;
	}
}

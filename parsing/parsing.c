/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:46:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/17 18:17:47 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// TODO: Free in case of malloc errors
t_list	*parse_line(const char *line, bool *success)
{
	size_t	i;
	t_list	*tokens;
	t_token	*token;
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
			token = ft_calloc(1, sizeof(t_token));
			if (token == NULL)
			{
				// TODO: Free what was malloced before (ft_lstclear??)
				*success = false;
				return (NULL);
			}
			token->start = i;
			token->type = QUOTED_STRING;
			token->expanded = true;
			i++;
			while (line[i] != '\0' && line[i] != '\'')
			{
				i++;
			}
			if (line[i] == '\0')
			{
				write_to_stderr("Parse Error: Unterminated string\n");
				*success = false;
				return (NULL);
			}
			else
				token->end = i;
			el = ft_lstnew(token);
			if (el == NULL)
			{
				// TODO: Free what was malloced before (ft_lstclear??)
				*success = false;
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		i++;
	}
	return (tokens);
}

void	print_tokens(t_list *tokens)
{
	t_token	*token;

	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->type == QUOTED_STRING)
			printf("Quoted string\nstart=(%zu)\nend=(%zu)\n", token->start, token->end);
		tokens = tokens->next;
	}
}

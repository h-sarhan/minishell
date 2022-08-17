/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 22:19:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/17 22:28:51 by hsarhan          ###   ########.fr       */
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
	token->end = i + 1;
	if (type == HEREDOC || type == APPEND_REDIR || type == LAST_EXIT)
		token->end++;
	token->substr = ft_substr(line, token->start, 1);
	if (token->substr == NULL)
		return (NULL);
	el = ft_lstnew(token);
	if (el == NULL)
		return (NULL);
	*idx = i + 1;
	if (type == HEREDOC || type == APPEND_REDIR || type == LAST_EXIT)
		*idx = i + 2;
	return (el);
}
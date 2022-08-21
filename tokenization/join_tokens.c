/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 11:25:46 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 09:36:26 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenization.h"

char	*join_tokens(t_list *tokens)
{
	t_token	*token;
	char	*res;

	res = ft_strdup("");
	while (tokens != NULL)
	{
		token = tokens->content;
		if (res[0] != '\0')
			res = strjoin_free(res, " ", 1);
		if (token->sub_tokens != NULL)
		{
			res = strjoin_free("(", res, 2);
			res = strjoin_free(res, join_tokens(token->sub_tokens), 3);
			res = strjoin_free(res, ")", 1);
		}
		else
			res = strjoin_free(res, token->substr, 1);
		tokens = tokens->next;
	}
	return (res);
}
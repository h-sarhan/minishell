/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 11:25:46 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/24 11:36:11 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Probably dont need this function anymore
char	*join_tokens(t_list *tokens)
{
	t_token	*token;
	char	*res;
	char	*subtoken_str;

	res = ft_strdup("");
	while (tokens != NULL)
	{
		token = tokens->content;
		if (res[0] != '\0')
			res = strjoin_free(res, " ", 1);
		if (token->type == SUB_EXPR)
		{
			subtoken_str = strjoin_free("(", join_tokens(token->sub_tokens), 2);
			subtoken_str = strjoin_free(subtoken_str, ")", 1);
			res = strjoin_free(res, subtoken_str, 3);
		}
		else
			res = strjoin_free(res, token->substr, 1);
		tokens = tokens->next;
	}
	return (res);
}
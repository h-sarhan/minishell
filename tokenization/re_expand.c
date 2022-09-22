/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   re_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:48:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/22 15:08:29 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void re_expand_tokens(t_shell *shell, t_list *tokens)
{
	t_token	*token;

	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->init_substr == NULL)
		{
			tokens = tokens->next;
			continue;
		}
		ft_free(&token->substr);
		token->substr = ft_strdup(token->init_substr);
		if (token->type == QUOTED_STRING || token->type == DOUBLE_QUOTED_STRING)
		{
			while (contains_env_var(token->substr))
				token->substr = expand_double_quote(shell, token->substr);
			token->substr = eat_quotes(token->substr);
		}
		if (token->type == ENV_VAR)
		{
			while (contains_env_var(token->substr))
				token->substr = expand_double_quote(shell, token->substr);
			// printf("substr after expanding is |%s|\n", token->substr);
			token->type = NORMAL;
			token->substr = eat_dollars(token->substr);
			token->substr = eat_quotes(token->substr);
		}
		if (token->type == NORMAL)
		{
			while (contains_env_var(token->substr))
				token->substr = expand_double_quote(shell, token->substr);
			if (ft_strchr(token->substr, '\'') != NULL || ft_strchr(token->substr, '\"') != NULL)
			{
				token->substr = eat_dollars(token->substr);
				token->substr = eat_quotes(token->substr);
			}
		}
		printf("Old substring is |%s|\n", token->init_substr);
		printf("New substring is |%s|\n", token->substr);
		tokens = tokens->next;
	}
}
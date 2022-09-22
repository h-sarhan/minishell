/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   re_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:48:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/22 15:31:17 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void re_expand_tokens(t_shell *shell, t_list *tokens)
{
	t_token	*token;
	// t_list	*el;

	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->init_substr == NULL)
		{
			tokens = tokens->next;
			continue;
		}
		ft_free(&token->substr);
		printf("Old substring is |%s|\n", token->init_substr);
		token->substr = ft_strdup(token->init_substr);
		if (token->type == QUOTED_STRING || token->type == DOUBLE_QUOTED_STRING)
		{
			while (contains_env_var(token->substr))
				token->substr = expand_double_quote(shell, token->substr);
			token->substr = eat_quotes(token->substr);
		}
		else if (token->type == ENV_VAR)
		{
			while (contains_env_var(token->substr))
				token->substr = expand_double_quote(shell, token->substr);
			// printf("substr after expanding is |%s|\n", token->substr);
			token->type = NORMAL;
			token->substr = eat_dollars(token->substr);
			token->substr = eat_quotes(token->substr);
		}
		else if (token->type == NORMAL)
		{
			while (contains_env_var(token->substr))
				token->substr = expand_double_quote(shell, token->substr);
			if (ft_strchr(token->substr, '\'') != NULL || ft_strchr(token->substr, '\"') != NULL)
			{
				token->substr = eat_dollars(token->substr);
				token->substr = eat_quotes(token->substr);
			}
		}
		else if (token->type == DUMMY)
		{
			while (contains_env_var(token->substr))
				token->substr = expand_double_quote(shell, token->substr);
			// printf("substr after expanding is |%s|\n", token->substr);
			token->type = NORMAL;
			token->substr = eat_dollars(token->substr);
			token->substr = eat_quotes(token->substr);
			if (ft_strlen(token->substr) != 0)
			{
				// char *substr_copy = ft_strdup(token->substr);
				// printf("%s\n", substr_copy);
				token->type = NORMAL;
				// bool success;
				// el = tokenize_line(shell, substr_copy, &success);
				// ft_free(&substr_copy);
				// ft_lstadd_back(&tokens, el);
			}
			else
			{
				token->type = DUMMY;
			}
		}
		printf("Old substring is |%s|\n", token->init_substr);
		printf("New substring is |%s|\n", token->substr);
		tokens = tokens->next;
	}
}
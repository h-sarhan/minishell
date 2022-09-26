/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 12:03:03 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/26 23:50:26 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	parse_subexpr(t_token *token, t_list **steps,
	t_list *tokens, bool *success)
{
	t_exec_step	*step;

	step = ft_calloc(1, sizeof(t_exec_step));
	if (step == NULL)
	{
		*success = false;
		return (false);
	}
	step->subexpr_line = ft_strdup(token->substr);
	ft_lstadd_back(steps, ft_lstnew(step));
	if (check_next_subexpr_token(tokens, &token, step, success) == false)
		return (false);
	return (true);
}

t_list	*parse_tokens(t_list *tokens, bool *success)
{
	t_token		*token;
	t_list		*steps;

	steps = NULL;
	*success = check_for_errors(tokens);
	if (*success == false)
		return (steps);
	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->type == SUB_EXPR)
			parse_subexpr(token, &steps, tokens, success);
		else if (is_terminator(token) == false)
			steps = parse_step(&tokens, &token, &steps, success);
		if (*success == false)
			return (steps);
		tokens = tokens->next;
	}
	ft_lstiter(steps, list_to_str_arr);
	*success = true;
	return (steps);
}

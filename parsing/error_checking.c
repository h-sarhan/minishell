/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_checking.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 22:18:08 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_for_errors(t_list *tokens)
{
	t_token	*token;
	t_token	*next_token;

	if (tokens == NULL)
		return (true);
	token = tokens->content;
	if (is_terminator(token) == true
		|| (ft_lstsize(tokens) == 1 && is_redirection(token))
		|| (is_terminator(ft_lstlast(tokens)->content) == true))
		return (false);
	while (tokens->next != NULL)
	{
		token = tokens->content;
		next_token = tokens->next->content;
		if (((is_terminator(token) && is_terminator(next_token)))
			|| (is_redirection(token) && is_redirection(next_token))
			|| (is_redirection(token) && is_terminator(next_token))
			|| (is_redirection(token) && next_token->expanded == true)
			|| (is_redirection(token) && next_token->type == SUB_EXPR)
			|| (token->type == SUB_EXPR && is_redirection(next_token))
			|| (token->type == PIPE && next_token->type == SUB_EXPR))
			return (false);
		tokens = tokens->next;
	}
	return (true);
}

bool	parsing_error(t_redir *redir, t_exec_step *step)
{
	ft_free(&redir);
	ft_lstclear(&step->cmd->args, free);
	ft_lstclear(&step->cmd->redirs, free_redir);
	return (false);
}

bool	check_next_subexpr_token(t_list *tokens, t_token **token,
	t_exec_step *step, bool *success)
{
	if (tokens->next != NULL)
	{
		*token = tokens->next->content;
		if ((*token)->type == PIPE)
		{
			*success = false;
			return (false);
		}
		if ((*token)->type == AND)
			step->and_next = true;
		if ((*token)->type == OR)
			step->or_next = true;
		if ((*token)->type == SUB_EXPR
			|| (*token)->type == DOUBLE_QUOTED_STRING
			|| (*token)->type == QUOTED_STRING || (*token)->type == NORMAL)
		{
			*success = false;
			return (false);
		}
	}
	return (true);
}

bool	check_next_token(t_list *cmd_end, t_token **token,
	t_exec_step *step, t_list *tokens)
{
	if (cmd_end->next != NULL)
	{
		*token = cmd_end->next->content;
		if ((*token)->type == PIPE)
			step->pipe_next = true;
		if ((*token)->type == AND)
			step->and_next = true;
		if ((*token)->type == OR)
			step->or_next = true;
		if ((*token)->type == SUB_EXPR)
			return (false);
		*token = tokens->next->next->content;
		if ((*token)->type == AND || (*token)->type == OR
			|| (*token)->type == PIPE)
			return (false);
	}
	return (true);
}

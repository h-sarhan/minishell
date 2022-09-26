/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 12:03:03 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/26 20:56:03 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_terminator(const t_token *token)
{
	if (token->type == PIPE
		|| token->type == AND
		|| token->type == OR)
		return (true);
	return (false);
}

static bool	is_redirection(const t_token *token)
{
	if (token->type == INPUT_REDIR || token->type == OUTPUT_REDIR
		|| token->type == APPEND || token->type == HEREDOC)
		return (true);
	return (false);
}

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

t_redir	*create_redir(t_token **token, t_list **start, t_exec_step *step)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
	redir->type = (*token)->type;
	*start = (*start)->next;
	if (*start == NULL || redir == NULL)
	{
		ft_free(&redir);
		ft_lstclear(&step->cmd->args, free);
		ft_lstclear(&step->cmd->redirs, free_redir);
		return (NULL);
	}
	*token = (*start)->content;
	if (is_redirection(*token) == true)
	{
		ft_free(&redir);
		ft_lstclear(&step->cmd->args, free);
		ft_lstclear(&step->cmd->redirs, free_redir);
		return (NULL);
	}
	if (redir->type != HEREDOC)
		redir->file = ft_strdup((*token)->substr);
	else
		redir->limiter = ft_strdup((*token)->substr);
	return (redir);
}


bool	parsing_error(t_redir *redir, t_exec_step *step)
{
	ft_free(&redir);
	ft_lstclear(&step->cmd->args, free);
	ft_lstclear(&step->cmd->redirs, free_redir);
	return (false);
}

bool	fill_exec_step(t_exec_step *step, t_list *start,
								const t_list *end)
{
	t_token	*tkn;
	t_redir	*redir;

	redir = NULL;
	while (start != NULL && start != end->next)
	{
		tkn = start->content;
		if (is_redirection(tkn) == true)
		{
			redir = create_redir(&tkn, &start, step);
			if (redir == NULL)
				return (false);
			ft_lstadd_back(&step->cmd->redirs, ft_lstnew(redir));
		}
		else if (tkn->type == DOUBLE_QUOTED_STRING
			|| tkn->type == QUOTED_STRING || tkn->type == NORMAL)
			ft_lstadd_back(&step->cmd->args, ft_lstnew(ft_strdup(tkn->substr)));
		else if (tkn->type == SUB_EXPR)
			return (parsing_error(redir, step));
		start = start->next;
	}
	return (true);
}

t_list	*parse_tokens(t_list *tokens, bool *success)
{
	t_token		*token;
	t_list		*steps;
	t_exec_step	*step;
	t_list		*cmd_start;
	t_list		*cmd_end;

	steps = NULL;
	*success = check_for_errors(tokens);
	if (*success == false)
		return (steps);
	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->type == SUB_EXPR)
		{
			step = ft_calloc(1, sizeof(t_exec_step));
			step->subexpr_line = ft_strdup(token->substr);
			ft_lstadd_back(&steps, ft_lstnew(step));
			if (tokens->next != NULL)
			{
				token = tokens->next->content;
				if (token->type == PIPE)
				{
					*success = false;
					return (steps);
				}
				if (token->type == AND)
					step->and_next = true;
				if (token->type == OR)
					step->or_next = true;
				if (token->type == SUB_EXPR
					|| token->type == DOUBLE_QUOTED_STRING
					|| token->type == QUOTED_STRING || token->type == NORMAL)
				{
					*success = false;
					return (steps);
				}
			}
		}
		else if (is_terminator(token) == false)
		{
			cmd_start = tokens;
			while (tokens->next != NULL
				&& is_terminator(tokens->next->content) == false)
			{
				token = tokens->content;
				if (token->type == SUB_EXPR)
				{
					*success = false;
					return (steps);
				}
				tokens = tokens->next;
			}
			cmd_end = tokens;
			step = ft_calloc(1, sizeof(t_exec_step));
			step->cmd = ft_calloc(1, sizeof(t_cmd));
			if (fill_exec_step(step, cmd_start, cmd_end) == false)
			{
				ft_free(&step->cmd);
				ft_free(&step);
				*success = false;
				return (steps);
			}
			if (cmd_end->next != NULL)
			{
				token = cmd_end->next->content;
				if (token->type == PIPE)
					step->pipe_next = true;
				if (token->type == AND)
					step->and_next = true;
				if (token->type == OR)
					step->or_next = true;
				if (token->type == SUB_EXPR)
				{
					*success = false;
					return (steps);
				}
				token = tokens->next->next->content;
				if (token->type == AND || token->type == OR
					|| token->type == PIPE)
				{
					*success = false;
					return (steps);
				}
			}
			ft_lstadd_back(&steps, ft_lstnew(step));
		}
		tokens = tokens->next;
	}
	ft_lstiter(steps, list_to_str_arr);
	*success = true;
	return (steps);
}

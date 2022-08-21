/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 12:03:03 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 22:36:10 by hsarhan          ###   ########.fr       */
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
	if (token->type == INPUT_REDIR || token->type == OUTPUT_REDIR || token->type == APPEND
		|| token->type == HEREDOC)
		return (true);
	return (false);
}

bool	check_for_errors(t_list *tokens)
{
	t_token	*token;

	token = tokens->content;
	if (is_terminator(token) == true)
		return (false);
	if (ft_lstsize(tokens) == 1 && is_redirection(token) == true)
		return (false);
	token = ft_lstlast(tokens)->content;
	if (is_terminator(token) == true)
		return (false);
	return (true);
}

bool	fill_exec_step(t_exec_step *step, t_list *start,
								const t_list *end)
{
	t_token	*token;
	t_redir	*redir;
	char	*cmd_arg;

	while (start != end->next)
	{
		token = start->content;
		if (is_redirection(token) == true)
		{
			redir = ft_calloc(1, sizeof(t_redir));
			redir->type = token->type;
			start = start->next;
			if (start == NULL)
			{
				// ??????
				ft_free(&redir);
				ft_lstclear(&step->cmd->args, free);
				ft_lstclear(&step->cmd->redirs, free_redir);
				return (false);
			}
			token = start->content;
			if (is_redirection(token) == true)
			{
				// ??????
				ft_free(&redir);
				ft_lstclear(&step->cmd->args, free);
				ft_lstclear(&step->cmd->redirs, free_redir);
				return (false);
			}
			if (redir->type != HEREDOC)
				redir->file = ft_strdup(token->substr);
			else
				redir->limiter = ft_strdup(token->substr);
			ft_lstadd_back(&step->cmd->redirs, ft_lstnew(redir));
		}
		else if (token->type == DOUBLE_QUOTED_STRING
				|| token->type == QUOTED_STRING || token->type == NORMAL)
		{
			cmd_arg = ft_strdup(token->substr);
			ft_lstadd_back(&step->cmd->args, ft_lstnew(cmd_arg));
		}
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
	if (check_for_errors(tokens) == false)
	{
		*success = false;
		return (steps);
	}
	while (tokens != NULL)
	{
		token = tokens->content;
		if (is_terminator(token) == false)
		{
			cmd_start = tokens;
			while (tokens->next != NULL
				&& is_terminator(tokens->next->content) == false)
				tokens = tokens->next;
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
				tokens = tokens->next;
				token = tokens->content;
			}
			ft_lstadd_back(&steps, ft_lstnew(step));
		}
		else if (token->type == AND || token->type == OR || token->type == PIPE)
		{
			*success = false;
			return (steps);
		}
		tokens = tokens->next;
	}
	*success = true;
	return (steps);
}

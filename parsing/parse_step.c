/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_step.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 22:20:48 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/29 09:56:29 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static bool	fill_exec_step(t_exec_step *step, t_list *start, const t_list *end)
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

static t_exec_step	*create_step(t_list *cmd_start, t_list *cmd_end,
	t_token **token, t_list *tokens)
{
	t_exec_step	*step;

	step = ft_calloc(1, sizeof(t_exec_step));
	if (step == NULL)
		return (NULL);
	step->cmd = ft_calloc(1, sizeof(t_cmd));
	if (step->cmd == NULL)
		return (NULL);
	if (fill_exec_step(step, cmd_start, cmd_end) == false)
	{
		ft_free(&step->cmd);
		ft_free(&step);
		return (NULL);
	}
	if (check_next_token(cmd_end, token, step, tokens) == false)
		return (NULL);
	return (step);
}

t_list	*parse_step(t_list **tokens, t_token **token, t_list **steps,
	bool *success)
{
	t_list		*cmd_start;
	t_exec_step	*step;

	cmd_start = *tokens;
	while ((*tokens)->next != NULL
		&& is_terminator((*tokens)->next->content) == false)
	{
		*token = (*tokens)->content;
		if ((*token)->type == SUB_EXPR)
		{
			*success = false;
			return (*steps);
		}
		(*tokens) = (*tokens)->next;
	}
	step = create_step(cmd_start, *tokens, token, *tokens);
	if (step == NULL)
	{
		*success = false;
		return (*steps);
	}
	ft_lstadd_back(steps, ft_lstnew(step));
	return (*steps);
}

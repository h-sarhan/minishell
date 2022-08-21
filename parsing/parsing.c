/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 12:03:03 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 17:03:12 by hsarhan          ###   ########.fr       */
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

void	fill_exec_step(t_exec_step *step, t_list *start,
								const t_list *end)
{
	t_token	*token;
	t_redir	*redir;
	char	*cmd_arg;

	while (start != end->next)
	{
		token = start->content;
		if (token->type == INPUT_REDIR || token->type == OUTPUT_REDIR || token->type == APPEND)
		{
			redir = ft_calloc(1, sizeof(t_redir));
			redir->type = token->type;
			start = start->next;
			token = start->content;
			redir->file = ft_strdup(token->substr);
			ft_lstadd_back(&step->cmd->redirs, ft_lstnew(redir));
		}
		else if (token->type == DOUBLE_QUOTED_STRING || token->type == QUOTED_STRING || token->type == NORMAL)
		{
			cmd_arg = ft_strdup(token->substr);
			ft_lstadd_back(&step->cmd->args, ft_lstnew(cmd_arg));
		}
		start = start->next;
	}
}

t_list	*parse_tokens(t_list *tokens)
{
	t_token		*token;
	t_list		*steps;
	t_exec_step	*step;
	t_list		*cmd_start;
	t_list		*cmd_end;

	steps = NULL;
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
			fill_exec_step(step, cmd_start, cmd_end);
			ft_lstadd_back(&steps, ft_lstnew(step));
		}
		tokens = tokens->next;
	}
	return (steps);
}

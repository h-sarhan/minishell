/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 12:03:03 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 13:13:35 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	is_operator(const t_token *token)
{
	if (token->type == PIPE
		|| token->type == INPUT_REDIR
		|| token->type == OUTPUT_REDIR
		|| token->type == APPEND
		|| token->type == HEREDOC
		|| token->type == PIPE
		|| token->type == AND
		|| token->type == OR)
		return (true);
	return (false);
}

t_list	*parse_tokens(t_list *tokens)
{
	t_token		*token;
	t_list		*steps;
	t_exec_step	*step;
	t_list		*cmd_start;
	t_list		*cmd_end;
	char		*cmds;

	steps = NULL;
	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->type == DOUBLE_QUOTED_STRING || token->type == QUOTED_STRING || token->type == NORMAL)
		{
			cmd_start = tokens;
			while (tokens->next != NULL && is_operator(tokens->next->content) == false)
				tokens = tokens->next;
			cmd_end = tokens;
			step = ft_calloc(1, sizeof(t_exec_step));
			cmds = ft_strdup("");
			while (cmd_start != cmd_end->next)
			{
				token = cmd_start->content;
				cmds = strjoin_free(cmds, token->substr, 1);
				cmds = strjoin_free(cmds, " ", 1);
				cmd_start = cmd_start->next;
			}
			step->cmd = ft_calloc(1, sizeof(t_cmd));
			step->cmd->args = ft_split(cmds, ' ');
			ft_free(&cmds);
			ft_lstadd_back(&steps, ft_lstnew(step));
			if (cmd_end->next != NULL)
			{
				token = cmd_end->next->content;
				if (token->type == PIPE)
					step->cmd->pipe = true;
				// if (token->type == PIPE)
			}
		}
		tokens = tokens->next;
	}
	return (steps);
}

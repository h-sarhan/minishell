/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:41:23 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*tokenize_operator_token(const char *line, size_t *i)
{
	t_list			*el;
	t_token_type	operator_type;

	if (line[*i] == '>' && line[*i + 1] != '>')
		operator_type = OUTPUT_REDIR;
	else if (line[*i] == '<' && line[*i + 1] != '<')
		operator_type = INPUT_REDIR;
	else if (line[*i] == '<' && line[*i + 1] == '<')
		operator_type = HEREDOC;
	else if (line[*i] == '>' && line[*i + 1] == '>')
		operator_type = APPEND;
	else if (line[*i] == '|' && line[*i + 1] != '|')
		operator_type = PIPE;
	else if (line[*i] == '&' && line[*i + 1] == '&')
		operator_type = AND;
	else
		operator_type = OR;
	el = tokenize_operator(line, i, operator_type);
	return (el);
}

void	*token_error(const char *msg, t_list **tokens, bool *success)
{
	*success = false;
	ft_lstclear(tokens, free_token);
	if (msg != NULL)
		ft_stderr(msg);
	return (NULL);
}

bool	last_token_was_heredoc(t_list *tokens)
{
	t_token	*token;

	if (tokens == NULL)
		return (false);
	token = ft_lstlast(tokens)->content;
	return (token->type == HEREDOC);
}

bool	tokenize_wildcard(const t_shell *shell, t_list **el,
	t_list **tokens, bool *success)
{
	t_token	*token;
	t_list	*wildcard_tokens;

	token = (*el)->content;
	token->substr = expand_wildcard(token->substr);
	token->expanded = true;
	if (ft_strchr(token->substr, '*') == NULL)
	{
		wildcard_tokens = tokenize_line(shell, token->substr, success);
		if (*success == false || wildcard_tokens == NULL)
		{
			ft_lstclear(el, free_token);
			ft_lstclear(tokens, free_token);
			ft_lstclear(&wildcard_tokens, free_token);
			return (*success);
		}
		token = wildcard_tokens->content;
		token->expanded = true;
		ft_lstclear(el, free_token);
		ft_lstadd_back(tokens, wildcard_tokens);
	}
	else
		ft_lstadd_back(tokens, *el);
	return (*success);
}

bool	check_for_token_errors(const char *line, bool *success)
{
	size_t	i;
	char	quote;
	bool	in_quotes;

	i = 0;
	in_quotes = false;
	quote = '\0';
	while (line[i] != '\0')
	{
		set_quotes(line[i], &quote, &in_quotes);
		if (!in_quotes && (line[i] == '\\' || line[i] == ';' || line[i] == '`'
				|| (line[i] == '&' && line[i + 1] != '&')
				|| (line[i] == '(' && line[i + 1] == ')')))
		{
			ft_stderr("Parse Error: Invalid input\n");
			*success = false;
			return (false);
		}
		if (!in_quotes && (line[i] == '&' && line[i + 1] != '\0'))
			i++;
		i++;
	}
	return (true);
}

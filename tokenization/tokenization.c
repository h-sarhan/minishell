/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:46:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/26 15:04:29 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_quotes(const char ch, char *quote, bool *in_quote)
{
	if (ch == '\'' || ch == '\"')
	{
		if (*quote == '\0')
		{
			*quote = ch;
			*in_quote = !(*in_quote);
		}
		else if (*quote == ch)
		{
			*in_quote = !(*in_quote);
			*quote = '\0';
		}
	}
}

static bool	last_token_was_heredoc(t_list *tokens)
{
	t_token	*token;

	if (tokens == NULL)
		return (false);
	token = ft_lstlast(tokens)->content;
	return (token->type == HEREDOC);
}

static bool	check_for_errors(const char *line, bool *success)
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
			write_to_stderr("Parse Error: Invalid input\n");
			*success = false;
			return (false);
		}
		if (!in_quotes && (line[i] == '&' && line[i + 1] != '\0'))
			i++;
		i++;
	}
	return (true);
}

static bool	is_operator(const char *line, const size_t i)
{
	if ((line[i] == '>' && line[i + 1] != '>')
		|| (line[i] == '<' && line[i + 1] != '<')
		|| (line[i] == '<' && line[i + 1] == '<')
		|| (line[i] == '>' && line[i + 1] == '>')
		|| (line[i] == '|' && line[i + 1] != '|')
		|| (line[i] == '&' && line[i + 1] == '&')
		|| (line[i] == '|' && line[i + 1] == '|'))
		return (true);
	return (false);
}

static t_list	*tokenize_operator_token(const char *line, size_t *i)
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

static void	tokenize_env_cleanup(const t_shell *shell, t_list **el,
	t_list **tokens, bool *success)
{
	t_token	*token;
	char	*substr_copy;

	token = (*el)->content;
	if (token->substr == NULL)
		ft_lstclear(el, free_token);
	else if (ft_strncmp(token->substr, "$\"\"", 3) == 0)
	{
		ft_free(&token->substr);
		token->substr = ft_strdup("");
		ft_lstadd_back(tokens, *el);
	}
	else if (ft_strlen(token->substr) != 0
		&& ft_strchr(token->substr, '$') != NULL)
		ft_lstadd_back(tokens, *el);
	else if (ft_strlen(token->substr) != 0)
	{
		substr_copy = ft_strdup(token->substr);
		ft_lstclear(el, free_token);
		*el = tokenize_line(shell, substr_copy, success);
		ft_free(&substr_copy);
		ft_lstadd_back(tokens, *el);
	}
	else
		ft_lstclear(el, free_token);
}

static bool	tokenize_wildcard(const t_shell *shell, t_list **el,
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

static void	*token_error(const char *msg, t_list **tokens, bool *success)
{
	*success = false;
	ft_lstclear(tokens, free_token);
	if (msg != NULL)
		write_to_stderr(msg);
	return (NULL);
}

t_list	*tokenize_line(const t_shell *shell, const char *line, bool *success)
{
	size_t	i;
	t_list	*tokens;
	t_list	*el;
	t_token	*tok;

	*success = true;
	if (check_for_errors(line, success) == false)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '\"' || is_operator(line, i) == true
			|| line[i] == '(')
		{
			if (line[i] == '\'')
				el = tokenize_single_quote(shell, line, &i);
			else if (line[i] == '\"')
				el = tokenize_double_quote(shell, line, &i);
			else if (is_operator(line, i) == true)
				el = tokenize_operator_token(line, &i);
			else
				el = tokenize_subexpr(shell, line, &i);
			if (el == NULL)
				return (token_error(NULL, &tokens, success));
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '$' && last_token_was_heredoc(tokens) == false)
		{
			el = tokenize_env_variable(shell, line, &i);
			if (el == NULL)
				return (token_error("Parse Error\n", &tokens, success));
			tokenize_env_cleanup(shell, &el, &tokens, success);
		}
		else if (line[i] == ')')
			return (token_error("Parse Error\n", &tokens, success));
		else if (line[i] != ' ')
		{
			el = tokenize_normal(shell, line, &i,
					last_token_was_heredoc(tokens) == false);
			if (el == NULL)
				return (token_error(NULL, &tokens, success));
			tok = el->content;
			if (ft_strchr(tok->substr, '*') != NULL)
			{
				if (tokenize_wildcard(shell, &el, &tokens, success) == false)
					return (NULL);
			}
			else
				ft_lstadd_back(&tokens, el);
		}
		if (line[i] != '\0')
			i++;
	}
	return (tokens);
}

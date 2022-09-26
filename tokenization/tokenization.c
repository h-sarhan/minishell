/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:46:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/26 12:47:47 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*tokenize_operator(const char *line, size_t *idx,
							const t_token_type type)
{
	t_token	*token;
	t_list	*el;

	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = *idx;
	token->type = type;
	token->end = *idx;
	if (type == HEREDOC || type == APPEND || type == LAST_EXIT
		|| type == AND || type == OR)
	{
		token->end++;
		token->substr = ft_substr(line, token->start, 2);
	}
	else
		token->substr = ft_substr(line, token->start, 1);
	el = ft_lstnew(token);
	if (token->substr == NULL || el == NULL)
		return (NULL);
	if (type == HEREDOC || type == APPEND || type == LAST_EXIT
		|| type == AND || type == OR)
		*idx += 1;
	return (el);
}

static t_token	*last_token(t_list *tokens)
{
	if (tokens == NULL)
		return (NULL);
	return (ft_lstlast(tokens)->content);
}

t_list	*tokenize_line(const t_shell *shell, const char *line, bool *success)
{
	size_t	i;
	t_list	*tokens;
	t_list	*el;
	bool	in_quotes;
	char	quote;
	t_token	*tok;
	t_token	*envvar_token;
	t_list	*wildcard_tokens;
	char	*substr_copy;

	in_quotes = false;
	quote = '\0';
	i = 0;
	*success = true;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (quote == '\0')
			{
				quote = line[i];
				in_quotes = !in_quotes;
			}
			else if (line[i] == quote)
			{
				quote = '\0';
				in_quotes = !in_quotes;
			}
		}
		if (!in_quotes && (line[i] == '\\' || line[i] == ';' || line[i] == '`'
				|| (line[i] == '&' && line[i + 1] != '&')
				|| (line[i] == '(' && line[i + 1] == ')')))
		{
			write_to_stderr("Parse Error: Invalid input\n");
			*success = false;
			return (NULL);
		}
		if (!in_quotes && (line[i] == '&' && line[i + 1] != '\0'))
			i++;
		i++;
	}
	tokens = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'')
		{
			el = tokenize_single_quote(shell, line, &i);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '\"')
		{
			el = tokenize_double_quote(shell, line, &i);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '$' && (tokens == NULL
				|| (tokens != NULL && last_token(tokens)->type != HEREDOC)))
		{
			el = tokenize_env_variable(shell, line, &i);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				write_to_stderr("Parse Error: Invalid input\n");
				return (NULL);
			}
			envvar_token = el->content;
			if (envvar_token->substr == NULL)
				ft_lstclear(&el, free_token);
			else if (ft_strncmp(envvar_token->substr, "$\"\"", 3) == 0)
			{
				ft_free(&envvar_token->substr);
				envvar_token->substr = ft_strdup("");
				ft_lstadd_back(&tokens, el);
			}
			else if (ft_strlen(envvar_token->substr) != 0
				&& ft_strchr(envvar_token->substr, '$') != NULL)
			{
				ft_lstadd_back(&tokens, el);
			}
			else if (ft_strlen(envvar_token->substr) != 0)
			{
				substr_copy = ft_strdup(envvar_token->substr);
				ft_lstclear(&el, free_token);
				el = tokenize_line(shell, substr_copy, success);
				ft_free(&substr_copy);
				ft_lstadd_back(&tokens, el);
			}
			else
				ft_lstclear(&el, free_token);
		}
		else if (line[i] == '>' && line[i + 1] != '>')
		{
			el = tokenize_operator(line, &i, OUTPUT_REDIR);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '<' && line[i + 1] != '<')
		{
			el = tokenize_operator(line, &i, INPUT_REDIR);
			if (el == NULL)
			{
				*success = false;
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '<' && line[i + 1] == '<')
		{
			el = tokenize_operator(line, &i, HEREDOC);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '>' && line[i + 1] == '>')
		{
			el = tokenize_operator(line, &i, APPEND);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '|' && line[i + 1] != '|')
		{
			el = tokenize_operator(line, &i, PIPE);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '&' && line[i + 1] == '&')
		{
			el = tokenize_operator(line, &i, AND);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '|' && line[i + 1] == '|')
		{
			el = tokenize_operator(line, &i, OR);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '(')
		{
			el = tokenize_subexpr(shell, line, &i);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == ')')
		{
			write_to_stderr("Parse Error: Invalid input\n");
			*success = false;
			ft_lstclear(&tokens, free_token);
			return (NULL);
		}
		else if (line[i] != ' ')
		{
			el = tokenize_normal(shell, line, &i,
					tokens != NULL && last_token(tokens)->type != HEREDOC);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				ft_lstclear(&el, free_token);
				return (NULL);
			}
			tok = el->content;
			if (ft_strchr(tok->substr, '*') != NULL)
			{
				tok->substr = expand_wildcard(tok->substr);
				tok->expanded = true;
				if (ft_strchr(tok->substr, '*') == NULL)
				{
					wildcard_tokens = tokenize_line(shell, tok->substr,
							success);
					if (*success == false || wildcard_tokens == NULL)
					{
						ft_lstclear(&el, free_token);
						ft_lstclear(&tokens, free_token);
						ft_lstclear(&wildcard_tokens, free_token);
						return (NULL);
					}
					tok = wildcard_tokens->content;
					tok->expanded = true;
					ft_lstclear(&el, free_token);
					ft_lstadd_back(&tokens, wildcard_tokens);
				}
				else
					ft_lstadd_back(&tokens, el);
			}
			else
				ft_lstadd_back(&tokens, el);
		}
		if (line[i] != '\0')
			i++;
	}
	return (tokens);
}

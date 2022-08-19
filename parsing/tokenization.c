/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:46:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/19 16:24:28 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

static t_list	*tokenize_env_var_helper(t_token *token)
{
	t_list	*el;
	char	*env_var;

	el = ft_lstnew(token);
	if (el == NULL)
		return (NULL);
	if (token->type == ENV_VAR)
	{
		env_var = token->substr;
		if (getenv(env_var) == NULL)
			token->substr = ft_strdup("");
		else
			token->substr = ft_strdup(getenv(env_var));
		ft_free(&env_var);
	}
	return (el);
}

t_list	*tokenize_env_variable(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*tkn;

	i = *idx + 1;
	tkn = ft_calloc(1, sizeof(t_token));
	if (tkn == NULL)
		return (NULL);
	tkn->start = i - 1;
	tkn->type = ENV_VAR;
	while (line[i] != '\0' && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	tkn->end = i - 1;
	if (tkn->start >= tkn->end)
	{
		tkn->type = NORMAL;
		tkn->substr = ft_substr(line, tkn->start, 1);
	}
	else
		tkn->substr = ft_substr(line, tkn->start + 1, tkn->end - tkn->start);
	if (tkn->substr == NULL)
		return (NULL);
	*idx = i - 1;
	return (tokenize_env_var_helper(tkn));
}

t_list	*tokenize_line(const char *line, bool *success)
{
	size_t	i;
	t_list	*tokens;
	t_list	*el;

	i = 0;
	*success = true;
	while (line[i] != '\0')
	{
		if (line[i] == '\\' || line[i] == ';' || line[i] == '`'
			|| (line[i] == '&' && line[i + 1] != '&')
			|| (line[i] == '(' && line[i + 1] == ')'))
		{
			write_to_stderr("Parse Error: Invalid input\n");
			*success = false;
			return (NULL);
		}
		if (line[i] == '&' && line[i + 1] != '\0')
			i++;
		i++;
	}
	tokens = NULL;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'')
		{
			el = tokenize_single_quote(line, &i);
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
			el = tokenize_double_quote(line, &i);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '$' && line[i + 1] != '?')
		{
			el = tokenize_env_variable(line, &i);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
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
		else if (line[i] == '$' && line[i + 1] == '?')
		{
			el = tokenize_operator(line, &i, LAST_EXIT);
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
			el = tokenize_subexpr(line, &i);
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
			el = tokenize_normal(line, &i);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		if (line[i] != '\0')
			i++;
	}
	return (tokens);
}

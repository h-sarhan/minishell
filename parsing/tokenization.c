/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:46:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/19 09:41:18 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_list	*tokenize_single_quote(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*token;
	t_list	*el;

	i = *idx;
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = i;
	token->type = QUOTED_STRING;
	i++;
	while (line[i] != '\0' && line[i] != '\'')
		i++;
	if (line[i] == '\0')
	{
		write_to_stderr("Parse Error: Unterminated string\n");
		return (NULL);
	}
	else
		token->end = i;
	token->substr = ft_substr(line, token->start + 1, token->end - token->start - 1);
	token->sub_tokens = NULL;
	if (token->substr == NULL)
		return (NULL);
	el = ft_lstnew(token);
	if (el == NULL)
		return (NULL);
	*idx = i;
	return (el);
}

static t_list	*tokenize_double_quote(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*token;
	t_list	*el;

	i = *idx;
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = i;
	token->type = DOUBLE_QUOTED_STRING;
	i++;
	while (line[i] != '\0' && line[i] != '\"')
		i++;
	if (line[i] == '\0')
	{
		write_to_stderr("Parse Error: Unterminated string\n");
		return (NULL);
	}
	else
		token->end = i;
	token->substr = ft_substr(line, token->start + 1, token->end - token->start - 1);
	token->sub_tokens = NULL;
	if (token->substr == NULL)
		return (NULL);
	el = ft_lstnew(token);
	if (el == NULL)
		return (NULL);
	*idx = i;
	while (contains_env_var(token->substr))
	{
		token->substr = expand_double_quote(token->substr);
	}
	return (el);
}

static t_list	*tokenize_env_variable(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*token;
	t_list	*el;
	char	*env_var;

	i = *idx;
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = i;
	token->type = ENV_VAR;
	i++;
	while (line[i] != '\0' && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	token->end = i - 1;
	// ? In this case the token would just be the dollar sign I think
	if (token->start >= token->end)
	{
		token->type = NORMAL;
		token->substr = ft_substr(line, token->start, 1);
	}
	else
		token->substr = ft_substr(line, token->start + 1, token->end - token->start);
	if (token->substr == NULL)
	{
		ft_free(&token);
		return (NULL);
	}
	token->sub_tokens = NULL;
	el = ft_lstnew(token);
	if (el == NULL)
	{
		ft_free(&token->substr);
		ft_free(&token);
		return (NULL);
	}
	*idx = i - 1;
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

// TODO: Free in case of malloc errors
t_list	*tokenize_line(const char *line, bool *success)
{
	size_t	i;
	t_list	*tokens;
	t_list	*el;

	// * Scanning for invalid characters
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
				// ? ft_lstclear here maybe??
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
				// ? ft_lstclear here maybe??
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
				// ? ft_lstclear here maybe??
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
				// ? ft_lstclear here maybe??
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
				// ? ft_lstclear here maybe??
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
				// ? ft_lstclear here maybe??
				*success = false;
				ft_lstclear(&tokens, free_token);
				return (NULL);
			}
			ft_lstadd_back(&tokens, el);
		}
		else if (line[i] == '>' && line[i + 1] == '>')
		{
			el = tokenize_operator(line, &i, APPEND_REDIR);
			if (el == NULL)
			{
				// ? ft_lstclear here maybe??
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
				// ? ft_lstclear here maybe??
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
				// ? ft_lstclear here maybe??
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
				// ? ft_lstclear here maybe??
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
				// ? ft_lstclear here maybe??
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
				// ? ft_lstclear here maybe??
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
		// TODO: Replace this with a is_whitespace function
		else if (line[i] != ' ')
		{
			el = tokenize_normal(line, &i);
			if (el == NULL)
			{
				// ? ft_lstclear here maybe??
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

void	print_tokens(t_list *tokens)
{
	t_token	*token;

	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->type == QUOTED_STRING)
		{
			printf("Single quoted string: \'%s\'\nstart=(%zu)\nend=(%zu)\n", token->substr, token->start,
					token->end);
		}
		if (token->type == DOUBLE_QUOTED_STRING)
		{
			printf("Double quoted string: \"%s\"\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == ENV_VAR)
		{
			printf("Environment Variable: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == INPUT_REDIR)
		{
			printf("Input redirection: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == OUTPUT_REDIR)
		{
			printf("Output redirection: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == APPEND_REDIR)
		{
			printf("Append redirection: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == PIPE)
		{
			printf("Pipe: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == LAST_EXIT)
		{
			printf("Last exit: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == HEREDOC)
		{
			printf("Heredoc: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == NORMAL)
		{
			printf("Normal: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == AND)
		{
			printf("AND token: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == OR)
		{
			printf("OR token: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		if (token->type == SUB_EXPR)
		{
			printf("Sub expression: (%s)\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
			printf("Sub expression START\n");
			print_tokens(token->sub_tokens);
			printf("Sub expression END\n");
		}
		if (token->type == WILDCARD)
		{
			printf("Wildcard token: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
					token->start, token->end);
		}
		tokens = tokens->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:46:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/01 12:40:22 by hsarhan          ###   ########.fr       */
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

// Rewrote this but I probably broke something
// static t_list	*tokenize_env_var_helper(const t_shell *shell, t_token *token)
// {
// 	bool	success;
// 	t_list	*expanded_tokens;
// 	t_list	*el;

// 	expanded_tokens = NULL;
// 	if (token->type == ENV_VAR)
// 	{
// 		while (contains_env_var(token->substr) == true)
// 			token->substr = expand_double_quote(shell, token->substr);
// 		if (*token->substr == '\0')
// 		{
// 			expanded_tokens = ft_lstnew(token);
// 			return (expanded_tokens);
// 		}
// 		expanded_tokens = tokenize_line(shell, token->substr, &success);
// 		free_token(token);
// 		if (success == false || expanded_tokens == NULL)
// 		{
// 			return (NULL);
// 		}
// 		return (expanded_tokens);
// 	}
// 	else
// 	{
// 		el = ft_lstnew(token);
// 		return (el);
// 	}
// }


char	*get_env_string(const char *line, size_t *idx)
{
	char	*str;
	size_t	i;
	bool	in_quote;
	char	quote;

	// * Stupid case: 
	// $"lkjvnfdlinfd liuhdf liufhnv *kuh df || "*'vdfuhvdfl'"vdskhbvfd"$GGGGGG" SSS "
	i = *idx;
	in_quote = false;
	quote = '\0';
	while (line[i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (quote == '\0')
			{
				quote = line[i];
				in_quote = !in_quote;

			}
			else if (quote == line[i])
			{
				in_quote = !in_quote;
				quote = '\0';
			}
		}
		if (line[i] == ' ' && in_quote == false)
			break ;
		i++;
	}
	if (in_quote == true)
	{
		*idx = i;
		return (NULL);
	}
	str = ft_substr(line, *idx, i);
	*idx = i;
	return (str);
}

t_list	*tokenize_env_variable(const t_shell *shell, const char *line, size_t *idx)
{
	t_token	*tkn;
	t_list	*el;

	tkn = ft_calloc(1, sizeof(t_token));
	if (tkn == NULL)
		return (NULL);
	// tkn->start = i - 1;
	tkn->type = ENV_VAR;
	tkn->substr = get_env_string(line, idx);
	if (tkn->substr == NULL)
		return (NULL);
	while (contains_env_var(tkn->substr))
		tkn->substr = expand_double_quote(shell, tkn->substr);
	// printf("%s\n", tkn->substr);
	tkn->type = NORMAL;
	tkn->substr = eat_quotes(tkn->substr);
	el = ft_lstnew(tkn);
	// tkn->end = i - 1;
	// if (tkn->start >= tkn->end)
	// {
	// 	tkn->type = NORMAL;
	// 	tkn->substr = ft_substr(line, tkn->start, 1);
	// }
	// else
	// 	tkn->substr = ft_substr(line, tkn->start + 1, tkn->end - tkn->start);
	return (el);
	
}
// t_list	*tokenize_env_variable(const t_shell *shell, const char *line, size_t *idx)
// {
// 	size_t	i;
// 	t_token	*tkn;
// 	t_list	*el;

// 	i = *idx + 1;
// 	tkn = ft_calloc(1, sizeof(t_token));
// 	if (tkn == NULL)
// 		return (NULL);
// 	tkn->start = i - 1;
// 	tkn->type = ENV_VAR;
// 	while (line[i] != '\0' && line[i] != '\"' && line[i] != '*' && line[i] != '\'' && line[i] != ' ')
// 		i++;
// 	tkn->end = i - 1;
// 	if (tkn->start >= tkn->end)
// 	{
// 		tkn->type = NORMAL;
// 		tkn->substr = ft_substr(line, tkn->start, 1);
// 	}
// 	else
// 		tkn->substr = ft_substr(line, tkn->start + 1, tkn->end - tkn->start);
// 	if (tkn->substr == NULL)
// 	{
// 		free_token(tkn);
// 		return (NULL);
// 	}
// 	*idx = i - 1;
// 	// Very stupid that I have to do this
// 	tkn->substr = strjoin_free("$", tkn->substr, 2);
// 	el = tokenize_env_var_helper(shell, tkn);
// 	tkn = ft_lstlast(el)->content;
// 	if (line[i] == '\'')
// 	{
// 		*idx = tkn->end + 1;
// 		free_token(tkn);
// 		el = tokenize_single_quote(line, idx);
// 		return (el);
// 	}
// 	if (line[i] == '\"')
// 	{
// 		// printf();
// 		if (line[i + 1] == '\"')
// 		{
// 			ft_free(&tkn->substr);
			
// 			tkn->substr = ft_strdup("");
// 			el = ft_lstnew(tkn);
// 			return (el);
// 		}
// 		*idx = tkn->end + 1;
// 		free_token(tkn);
// 		el = tokenize_double_quote(shell, line, idx);
// 		return (el);
// 	}
// 	if (line[i] != '\0')
// 	{
// 		tkn = ft_lstlast(el)->content;
// 		size_t	start;
// 		start = i;
// 		while (line[i] != ' ' && line[i] != '\0')
// 			i++;
// 		tkn->substr = strjoin_free(tkn->substr, eat_quotes(ft_substr(line, start, i - start + 1)), 3);
// 		if (tkn->substr == NULL)
// 		{
// 			ft_lstclear(&el, free_token);
// 			return (NULL);
// 		}
// 		// ! SHIT CODE
// 		if (tkn->substr[0] == '$' && (ft_isalnum(tkn->substr[1]) || tkn->substr[1] == '_'))
// 			tkn->substr = substr_free(tkn->substr, 1, ft_strlen(tkn->substr) - 1);
// 		*idx = i - 1;
// 	}
// 	// if (el == NULL)
// 	// {
// 	// 	printf("Returning NULL\n");
// 	// }
// 	return (el);
// }

t_list	*tokenize_line(const t_shell *shell, const char *line, bool *success)
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
			el = tokenize_double_quote(shell, line, &i);
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
			el = tokenize_env_variable(shell, line, &i);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				write_to_stderr("Parse Error: Invalid input\n");
				return (NULL);
			}
			t_token	*envvar_token = el->content;
			if (*envvar_token->substr != '\0')
				ft_lstadd_back(&tokens, el);
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
			el = tokenize_normal(shell, line, &i);
			if (el == NULL)
			{
				*success = false;
				ft_lstclear(&tokens, free_token);
				ft_lstclear(&el, free_token);
				return (NULL);
			}
			t_token	*tok = el->content;
			if (ft_strchr(tok->substr, '*') != NULL && tok->expanded == false)
			{
				tok->substr = expand_wildcard(tok->substr);
				tok->expanded = true;
				if (ft_strchr(tok->substr, '*') == NULL)
				{
					t_list	*wildcard_tokens = tokenize_line(shell, tok->substr, success);
					if (*success == false || wildcard_tokens == NULL)
					{
						ft_lstclear(&el, free_token);
						ft_lstclear(&tokens, free_token);
						ft_lstclear(&wildcard_tokens, free_token);
						return (NULL);
					}
					tok = wildcard_tokens->content;
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

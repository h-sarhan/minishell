/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:46:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/22 14:52:06 by hsarhan          ###   ########.fr       */
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

static char	*get_env_string(const char *line, size_t *idx)
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
		if ((line[i] == ' ' || ft_strchr("<>|(&", line[i]) != NULL) && in_quote == false)
			break ;
		i++;
	}
	if (in_quote == true)
	{
		*idx = i;
		return (NULL);
	}
	str = ft_substr(line, *idx, i - *idx);
	*idx = i - 1;
	return (str);
}

char *eat_dollars(const char *str)
{
	size_t	num_dollars;
	size_t	i;
	size_t	j;
	char	*trimmed_str;
	char	quote;
	bool	in_quote;

	in_quote = false;
	num_dollars = 0;
	i = 0;
	while (str[i] != '\0' && str[i + 1] != '\0')
	{
		if (str[i] == '$' && (str[i + 1] == '\'' || str[i + 1] == '\"'))
		{
			quote = str[i + 1];
			num_dollars += 1;
			i += 2;
			while (str[i] != quote && str[i] != '\0')
				i++;
			if (str[i] == '\0')
			{
				ft_free(&str);
				return (NULL);
			}
			i++;
		}
		else
			i++;
	}
	trimmed_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (trimmed_str == NULL)
		return (NULL);
	i = 0;
	j = 0;
	quote = '\0';
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (quote == '\0')
			{
				quote = str[i];
				in_quote = !in_quote;
			}
			else if (quote == str[i])
			{
				in_quote = !in_quote;
				quote = '\0';
			}
		}
		if (in_quote == true)
		{

			trimmed_str[j] = str[i];
			j++;
			i++;
		}
		else if (str[i] == '$' && (str[i + 1] == '\'' || str[i + 1] == '\"'))
		{
			quote = str[i + 1];
			trimmed_str[j] = str[i];
			i++;
		}
		else
		{
			trimmed_str[j] = str[i];
			j++;
			i++;
		}
	}
	ft_free(&str);
	return (trimmed_str);
}

t_list	*tokenize_env_variable(const t_shell *shell, const char *line, size_t *idx)
{
	t_token	*tkn;
	t_list	*el;

	tkn = ft_calloc(1, sizeof(t_token));
	if (tkn == NULL)
		return (NULL);
	tkn->type = ENV_VAR;
	tkn->substr = get_env_string(line, idx);
	// printf("substr is |%s|\n", tkn->substr);
	if (tkn->substr == NULL)
		return (NULL);
	tkn->init_substr = ft_strdup(tkn->substr);
	if (ft_strncmp(tkn->substr, "$\"\"", ft_strlen(tkn->substr)) == 0 && ft_strlen(tkn->substr) == 3)
	{
		tkn->type = NORMAL;
		el = ft_lstnew(tkn);
		return (el);
	}
	while (contains_env_var(tkn->substr))
		tkn->substr = expand_double_quote(shell, tkn->substr);
	// printf("substr after expanding is |%s|\n", tkn->substr);
	
	tkn->type = NORMAL;
	tkn->substr = eat_dollars(tkn->substr);
	tkn->substr = eat_quotes(tkn->substr);
	el = ft_lstnew(tkn);
	return (el);
	
}

t_list	*tokenize_line(const t_shell *shell, const char *line, bool *success)
{
	size_t	i;
	t_list	*tokens;
	t_list	*el;
	bool	in_quotes = false;
	char	quote = '\0';
	
	i = 0;
	*success = true;
	// ! Add in quotes check
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
		// else if (line[i] == '$' && line[i + 1] != '?')
		else if (line[i] == '$')
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
			if (ft_strncmp(envvar_token->substr, "$\"\"", 3) == 0)
			{
				ft_free(&envvar_token->substr);
				envvar_token->substr = ft_strdup("");
				ft_lstadd_back(&tokens, el);
			}
			else if (ft_strlen(envvar_token->substr) != 0 && ft_strchr(envvar_token->substr, '$') != NULL)
			{
				ft_lstadd_back(&tokens, el);
			}
			else if (ft_strlen(envvar_token->substr) != 0)
			{
				char *substr_copy = ft_strdup(envvar_token->substr);
				// printf("%s\n", substr_copy);
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
		// else if (line[i] == '$' && line[i + 1] == '?')
		// {
		// 	el = tokenize_operator(line, &i, LAST_EXIT);
		// 	if (el == NULL)
		// 	{
		// 		*success = false;
		// 		ft_lstclear(&tokens, free_token);
		// 		return (NULL);
		// 	}
		// 	ft_lstadd_back(&tokens, el);
		// }
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

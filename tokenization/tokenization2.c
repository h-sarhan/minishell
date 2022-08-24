/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 22:19:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/24 13:57:40 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	*parse_error(const char *msg)
{
	write_to_stderr(msg);
	return (NULL);
}

char	*eat_quotes(char *str)
{
	size_t	num_quotes;
	size_t	i;
	size_t	j;
	char	*trimmed_str;

	num_quotes = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
			num_quotes++;
		i++;
	}
	trimmed_str = ft_calloc(ft_strlen(str) - num_quotes + 1, sizeof(char));
	if (trimmed_str == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] != '\'' && str[i] != '\"')
		{
			trimmed_str[j] = str[i];
			i++;
			j++;
		}
		else
			i++;
	}
	ft_free(&str);
	return (trimmed_str);
}

t_list	*tokenize_normal(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*tkn;
	t_list	*el;
	char	quote;
	
	quote = '\0';
	i = *idx;
	tkn = ft_calloc(1, sizeof(t_token));
	if (tkn == NULL)
		return (NULL);
	tkn->start = i;
	tkn->type = NORMAL;
	while (line[i] != '\0' && ft_strchr(" \'\"$<>|(&", line[i]) == NULL)
		i++;
	while (line[i] == '\'' || line[i] == '\"')
	{
		quote = line[i];
		i++;
		while (line[i] != '\0' && line[i] != quote)
			i++;
		if (line[i] == '\0')
		{
			free_token(tkn);
			return (NULL);
		}
		i++;
		while (line[i] != '\0' && ft_strchr(" \'\"$<>|(&", line[i]) == NULL)
			i++;
	}
	tkn->end = i - 1;
	tkn->substr = ft_substr(line, tkn->start, tkn->end - tkn->start + 1);
	if (quote != '\0')
		tkn->substr = eat_quotes(tkn->substr);
	if (tkn->substr == NULL)
		return (NULL);
	el = ft_lstnew(tkn);
	*idx = tkn->end;
	return (el);
}

static t_list	*tokenize_subexpr_helper(t_token *tkn, const size_t i,
					const char *line, size_t *idx)
{
	bool	success;
	t_list	*el;

	tkn->end = i - 1;
	tkn->substr = ft_substr(line, tkn->start + 1, tkn->end - tkn->start - 1);
	if (tkn->substr == NULL)
		return (NULL);
	success = true;
	tkn->sub_tokens = tokenize_line(tkn->substr, &success);
	if (success == false)
		return (NULL);
	el = ft_lstnew(tkn);
	*idx = tkn->end;
	return (el);
}

t_list	*tokenize_subexpr(const char *line, size_t *idx)
{
	size_t	i;
	t_token	*token;
	int		paren_counter;

	i = *idx + 1;
	token = ft_calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->start = i - 1;
	token->type = SUB_EXPR;
	paren_counter = 1;
	while (line[i] != '\0' && paren_counter != 0)
	{
		if (line[i] == '(')
			paren_counter++;
		else if (line[i] == ')')
			paren_counter--;
		i++;
	}
	if (paren_counter != 0)
		return (parse_error("Parse Error: Invalid input\n"));
	return (tokenize_subexpr_helper(token, i, line, idx));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_normal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 12:17:53 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/28 20:27:29 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	*parse_error(const char *msg, t_token *tkn)
{
	free_token(tkn);
	if (msg != NULL)
		ft_stderr(msg);
	return (NULL);
}

static t_token	*skip_to_end_token(const char *line, size_t *i, t_token *tkn)
{
	char	quote;

	quote = '\0';
	while ((line[*i] == '\'' || line[*i] == '\"') && line[*i] != '\0')
	{
		quote = line[*i];
		*i += 1;
		while (line[*i] != '\0' && line[*i] != quote)
			*i += 1;
		if (line[*i] == '\0')
			return (parse_error("Parse Error: Invalid input\n", tkn));
		*i += 1;
		while (line[*i] != '\0' && ft_strchr(" \'\"$<>|(&)", line[*i]) == NULL)
			*i += 1;
		tkn->expanded = true;
	}
	tkn->end = *i - 1;
	tkn->substr = ft_substr(line, tkn->start, tkn->end - tkn->start + 1);
	return (tkn);
}

t_list	*tokenize_normal(const t_shell *shell, const char *line, size_t *idx,
	bool expand_var)
{
	size_t	i;
	t_token	*tkn;
	t_list	*el;

	i = *idx;
	tkn = ft_calloc(1, sizeof(t_token));
	if (tkn == NULL)
		return (NULL);
	tkn->start = i;
	tkn->type = NORMAL;
	while (line[i] != '\0' && ft_strchr(" \'\"<>|(&)", line[i]) == NULL)
		i++;
	if (line[i] == ')')
		return (parse_error("Parse Error: Invalid input\n", tkn));
	if (skip_to_end_token(line, &i, tkn) == NULL)
		return (NULL);
	while (expand_var == true && contains_env_var(tkn->substr))
		tkn->substr = expand_env_var(shell, tkn->substr);
	tkn->substr = eat_dollars(tkn->substr);
	tkn->substr = eat_quotes(tkn->substr);
	if (tkn->substr == NULL)
		return (parse_error(NULL, tkn));
	el = ft_lstnew(tkn);
	*idx = tkn->end;
	return (el);
}

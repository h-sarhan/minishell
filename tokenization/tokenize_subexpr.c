/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 22:19:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/26 12:18:55 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	*parse_error(const char *msg, t_token *tkn)
{
	free_token(tkn);
	if (msg != NULL)
		write_to_stderr(msg);
	return (NULL);
}

static t_list	*tokenize_subexpr_helper(const t_shell *shell, t_token *tkn,
	const char *line, size_t *idx)
{
	bool	success;
	t_list	*el;

	tkn->substr = ft_substr(line, tkn->start + 1, tkn->end - tkn->start - 1);
	if (tkn->substr == NULL)
		return (NULL);
	success = true;
	tkn->sub_tokens = tokenize_line(shell, tkn->substr, &success);
	if (success == false)
		return (NULL);
	el = ft_lstnew(tkn);
	*idx = tkn->end;
	return (el);
}

t_list	*tokenize_subexpr(const t_shell *shell, const char *line, size_t *idx)
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
		return (parse_error("Parse Error: Invalid input\n", token));
	token->end = i - 1;
	return (tokenize_subexpr_helper(shell, token, line, idx));
}

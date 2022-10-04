/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_wildcard2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 09:21:09 by hsarhan           #+#    #+#             */
/*   Updated: 2022/10/04 09:22:17 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_quotes_count(const char *str, size_t *num_words, size_t *i)
{
	char	quote;

	if (str[*i] == '\'' || str[*i] == '\"')
	{
		quote = str[*i];
		*i += 1;
		while (str[*i] != quote && str[*i] != '\0')
			*i += 1;
		*num_words += 1;
		*i += 1;
		return (true);
	}
	return (false);
}

bool	handle_quotes_split(const char *wc, size_t *i, size_t *word_count,
	t_wildcard **wc_split)
{
	char		quote;
	char		*wc_seg;
	size_t		start;

	if (wc[*i] == '\'' || wc[*i] == '\"')
	{
		quote = wc[*i];
		start = *i;
		*i += 1;
		while (wc[*i] != quote && wc[*i] != '\0')
			*i += 1;
		wc_seg = eat_quotes(create_word(wc, start, *i));
		if (wc_seg != NULL && wc_seg[0] != '\0')
		{
			wc_split[*word_count] = create_wc(wc_seg, false);
			*word_count += 1;
		}
		else
			ft_free(&wc_seg);
		*i += 1;
		return (true);
	}
	return (false);
}

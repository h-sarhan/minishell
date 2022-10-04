/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 10:03:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/10/04 08:59:24 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_quotes_count(const char *str, size_t *num_words, size_t *i)
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

static int	count_segments(char const *str)
{
	size_t	i;
	size_t	num_words;

	i = 0;
	num_words = 0;
	while (str[i] != '\0')
	{
		if (handle_quotes_count(str, &num_words, &i) == true)
			continue ;
		if (str[i] == '*')
			num_words++;
		while (str[i] == '*' && str[i] != '\0')
			i++;
		if (str[i] == '\0')
			break ;
		num_words++;
		while (str[i] != '*' && str[i] != '\0')
			i++;
	}
	return (num_words);
}

static char	*create_word(char const *str, const size_t word_start,
							const size_t word_end)
{
	char	*word;

	word = ft_calloc((word_end - word_start + 2), sizeof(char));
	if (word == NULL)
		return (NULL);
	ft_strlcpy(word, &str[word_start], word_end - word_start + 2);
	return (word);
}

static t_wildcard	*create_wc(char *str, bool is_wildcard)
{
	t_wildcard	*wildcard;

	wildcard = ft_calloc(1, sizeof(t_wildcard));
	if (wildcard == NULL)
		return (NULL);
	wildcard->str = str;
	wildcard->is_wildcard = is_wildcard;
	return (wildcard);
}

static bool	handle_quotes_split(const char *wc, size_t *i, size_t *word_count,
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

t_wildcard	**split_wildcard(const char *wc)
{
	t_wildcard	**wc_split;
	size_t		i;
	size_t		start;
	size_t		num_words;

	wc_split = ft_calloc((count_segments(wc) + 1), sizeof(t_wildcard *));
	if (wc_split == NULL)
		return (NULL);
	i = 0;
	num_words = 0;
	while (wc[i] != '\0')
	{
		if (handle_quotes_split(wc, &i, &num_words, wc_split) == true)
			continue ;
		if (wc[i] == '*')
			wc_split[num_words++] = create_wc(ft_strdup("*"), true);
		while (wc[i] == '*' && wc[i] != '\0')
			i++;
		if (wc[i] == '\0')
			break ;
		if (wc[i] == '\'' || wc[i] == '\"')
			continue ;
		start = i;
		while (wc[i] != '*' && wc[i] != '\0')
			i++;
		wc_split[num_words++] = create_wc(create_word(wc, start, i - 1), false);
	}
	wc_split[num_words] = NULL;
	return (wc_split);
}

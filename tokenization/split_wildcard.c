/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 10:03:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/10/03 20:36:39 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_segments(char const *str)
{
	int	i;
	int	num_words;

	i = 0;
	num_words = 0;
	while (str[i] != '\0')
	{
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

static t_wildcard	*create_wildcard(char *str, bool is_wildcard)
{
	t_wildcard	*wildcard;

	wildcard = ft_calloc(1, sizeof(t_wildcard));
	if (wildcard == NULL)
		return (NULL);
	wildcard->str = str;
	wildcard->is_wildcard = is_wildcard;
	return (wildcard);
}

t_wildcard	**split_wildcard(const char *wc)
{
	t_wildcard	**split_wildcard;
	size_t		i;
	size_t		word_start;
	size_t		word_count;
	char		quote;

	split_wildcard = ft_calloc((count_segments(wc) + 1), sizeof(t_wildcard *));
	if (split_wildcard == NULL)
		return (NULL);
	i = 0;
	word_count = 0;
	while (wc[i] != '\0')
	{
		if (wc[i] == '\'' || wc[i] == '\"')
		{
			quote = wc[i];
			word_start = i;
			i++;
			while (wc[i] != quote && wc[i] != '\0')
				i++;
			char	*wc_seg = eat_quotes(create_word(wc, word_start, i));
			if (wc_seg != NULL && wc_seg[0] != '\0')
				split_wildcard[word_count++] = create_wildcard(wc_seg, false);
			else
				ft_free(&wc_seg);
			i++;
			continue;
		}
		if (wc[i] == '*')
			split_wildcard[word_count++] = create_wildcard(ft_strdup("*"), true);
		while (wc[i] == '*' && wc[i] != '\0')
			i++;
		if (wc[i] == '\0')
			break ;
		if (wc[i] == '\'' || wc[i] == '\"')
			continue;
		word_start = i;
		while (wc[i] != '*' && wc[i] != '\0')
			i++;
		split_wildcard[word_count++] = create_wildcard(create_word(wc, word_start, i - 1), false);
	}
	split_wildcard[word_count] = NULL;
	return (split_wildcard);
}

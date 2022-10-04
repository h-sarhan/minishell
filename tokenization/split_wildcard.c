/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 10:03:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/10/04 09:24:23 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_wildcard	**create_wc_split(char const *str)
{
	size_t		i;
	size_t		num_words;
	t_wildcard	**wc_split;

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
	wc_split = ft_calloc((num_words + 1), sizeof(t_wildcard *));
	if (wc_split == NULL)
		return (NULL);
	return (wc_split);
}

char	*create_word(char const *str, const size_t word_start,
							const size_t word_end)
{
	char	*word;

	word = ft_calloc((word_end - word_start + 2), sizeof(char));
	if (word == NULL)
		return (NULL);
	ft_strlcpy(word, &str[word_start], word_end - word_start + 2);
	return (word);
}

t_wildcard	*create_wc(char *str, bool is_wildcard)
{
	t_wildcard	*wildcard;

	wildcard = ft_calloc(1, sizeof(t_wildcard));
	if (wildcard == NULL)
		return (NULL);
	wildcard->str = str;
	wildcard->is_wildcard = is_wildcard;
	return (wildcard);
}

static void	add_non_wc_segment(const char *wc, size_t *i, size_t *num_words,
	t_wildcard **wc_split)
{
	size_t	start;

	start = *i;
	while (wc[*i] != '*' && wc[*i] != '\0')
		*i += 1;
	wc_split[*num_words] = create_wc(create_word(wc, start, *i - 1), false);
	*num_words += 1;
}

t_wildcard	**split_wildcard(const char *wc)
{
	t_wildcard	**wc_split;
	size_t		i;
	size_t		num_words;

	wc_split = create_wc_split(wc);
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
		add_non_wc_segment(wc, &i, &num_words, wc_split);
	}
	wc_split[num_words] = NULL;
	return (wc_split);
}

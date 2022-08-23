/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 10:03:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 12:09:20 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	**split_wildcard(const char *wc)
{
	char	**split_wildcard;
	size_t	i;
	size_t	word_start;
	size_t	word_count;

	split_wildcard = ft_calloc((count_segments(wc) + 1), sizeof(char *));
	if (split_wildcard == NULL)
		return (NULL);
	i = 0;
	word_count = 0;
	while (wc[i] != '\0')
	{
		if (wc[i] == '*')
			split_wildcard[word_count++] = ft_strdup("*");
		while (wc[i] == '*' && wc[i] != '\0')
			i++;
		if (wc[i] == '\0')
			break ;
		word_start = i;
		while (wc[i] != '*' && wc[i] != '\0')
			i++;
		split_wildcard[word_count++] = create_word(wc, word_start, i - 1);
	}
	split_wildcard[word_count] = NULL;
	return (split_wildcard);
}

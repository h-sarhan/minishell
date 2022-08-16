/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:01:09 by hsarhan           #+#    #+#             */
/*   Updated: 2022/06/20 12:12:42 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *str, char sep)
{
	int	i;
	int	num_words;

	i = 0;
	num_words = 0;
	while (str[i] != '\0')
	{
		while (str[i] == sep && str[i] != '\0')
			i++;
		if (str[i] == '\0')
			break ;
		num_words++;
		while (str[i] != sep && str[i] != '\0')
			i++;
	}
	return (num_words);
}

static char	*create_word(char const *str, int start, int end)
{
	char	*word;

	word = ft_calloc(end - start + 2, sizeof(char));
	if (word == NULL)
		return (NULL);
	ft_strlcpy(word, &str[start], end - start + 2);
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**words;
	int		i;
	int		word_start;
	int		wc;

	words = ft_calloc(count_words(s, c) + 1, sizeof(char *));
	if (words == NULL)
		return (NULL);
	i = 0;
	wc = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (s[i] == '\0')
			break ;
		word_start = i;
		while (s[i] != c && s[i] != '\0')
			i++;
		words[wc] = create_word(s, word_start, i - 1);
		if (words[wc++] == NULL)
			return (NULL);
	}
	words[wc] = NULL;
	return (words);
}

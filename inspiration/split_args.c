/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:02:21 by hsarhan           #+#    #+#             */
/*   Updated: 2022/07/04 13:26:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Increments i until we reach the end quote
static int	skip_to_end_quote(const char *str, char quote)
{
	int	i;

	i = 1;
	while (str[i] != '\0')
	{
		if (str[i] == quote)
			break ;
		i++;
	}
	return (i);
}

// Counts the arguments in a string
static int	count_args(char const *str, char sep)
{
	int	i;
	int	num_words;

	i = 0;
	num_words = 0;
	while (str[i] != '\0')
	{
		while (str[i] == sep && str[i] != '\'' && str[i] != '\"')
			i++;
		if (str[i] == '\'' || str[i] == '\"')
			i += skip_to_end_quote(&str[i], str[i]);
		if (str[i] == '\0')
			break ;
		num_words++;
		while (str[i] != sep && str[i] != '\0')
			i++;
	}
	return (num_words);
}

// Same as ft_split except that we always treat whatever is inside
// quotation marks as one word
char	**split_args(char const *s, char c)
{
	char	**words;
	int		i;
	int		word_start;
	int		wc;

	words = ft_calloc(count_args(s, c) + 1, sizeof(char *));
	if (words == NULL)
		return (NULL);
	i = 0;
	wc = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\'' && s[i] != '\"')
			i++;
		if (s[i] == '\0')
			break ;
		word_start = i;
		if (s[i] == '\'' || s[i] == '\"')
			i += skip_to_end_quote(&s[i], s[i]);
		while (s[i] != c && s[i] != '\0')
			i++;
		words[wc++] = ft_substr(s, word_start, i - word_start);
	}
	words[wc] = NULL;
	return (words);
}

// I ended up not using these two functions
// since interpreting backslash characters is not even required by minishell

// Escapes quotes in a string
// static char	*escape_quotes(char *str)
// {
// 	int		i;
// 	char	*escaped_str;
// 	int		j;

// 	if (str == NULL)
// 		return (NULL);
// 	escaped_str = ft_calloc(count_chars(str) + 1, sizeof(char));
// 	if (escaped_str == NULL)
// 		exit(EXIT_FAILURE);
// 	i = 0;
// 	j = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '\\' && ft_strchr("\"\'\\", str[i + 1]))
// 		{
// 			escaped_str[j++] = str[i + 1];
// 			i += 2;
// 		}
// 		else
// 			escaped_str[j++] = str[i++];
// 	}
// 	free(str);
// 	return (escaped_str);
// }

// Counts characters that wont be escaped
// static int	count_chars(char *str)
// {
// 	int		i;
// 	int		num_chars;

// 	i = 0;
// 	num_chars = 0;
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == '\\' && ft_strchr("\"\'\\", str[i + 1]))
// 			i += 2;
// 		else
// 			i++;
// 		num_chars++;
// 	}
// 	return (num_chars);
// }

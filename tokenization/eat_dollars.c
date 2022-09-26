/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_dollars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 12:47:56 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/26 17:43:17 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// static void	set_quotes(const char ch, char *quote, bool *in_quote)
// {
// 	if (ch == '\'' || ch == '\"')
// 	{
// 		if (*quote == '\0')
// 		{
// 			*quote = ch;
// 			*in_quote = !(*in_quote);
// 		}
// 		else if (*quote == ch)
// 		{
// 			*in_quote = !(*in_quote);
// 			*quote = '\0';
// 		}
// 	}
// }

static void	skip_dollar(const char *str, char *trimmed, char *quote, size_t *i)
{
	*quote = str[*i + 1];
	*i += 1;
	*trimmed = str[*i];
}

char	*eat_dollars(const char *str)
{
	size_t	i;
	size_t	j;
	char	*trimmed_str;
	char	quote;
	bool	in_quote;

	trimmed_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (trimmed_str == NULL)
		return (NULL);
	i = 0;
	j = 0;
	quote = '\0';
	in_quote = false;
	while (str[i] != '\0')
	{
		set_quotes(str[i], &quote, &in_quote);
		if (in_quote == true)
			trimmed_str[j++] = str[i++];
		else if (str[i] == '$' && (str[i + 1] == '\'' || str[i + 1] == '\"'))
			skip_dollar(str, &trimmed_str[j], &quote, &i);
		else
			trimmed_str[j++] = str[i++];
	}
	ft_free(&str);
	return (trimmed_str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 12:12:00 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/26 12:12:52 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_quotes(const char *str)
{
	int		num_quotes;
	size_t	i;
	char	quote;

	num_quotes = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i++];
			num_quotes += 2;
			while (str[i] != quote && str[i] != '\0')
				i++;
			if (str[i] == '\0')
			{
				ft_free(&str);
				return (-1);
			}
			i++;
		}
		else
			i++;
	}
	return (num_quotes);
}

static void	skip_quotes(const char *str, char *trimmed_str)
{
	size_t	i;
	size_t	j;
	char	quote;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i];
			i++;
			while (str[i] != quote)
			{
				trimmed_str[j++] = str[i++];
			}
			i++;
		}
		else
		{
			trimmed_str[j++] = str[i++];
		}
	}
}

char	*eat_quotes(const char *str)
{
	char	*trimmed_str;

	if (count_quotes(str) == -1)
		return (NULL);
	trimmed_str = ft_calloc(ft_strlen(str) - count_quotes(str) + 1, 1);
	if (trimmed_str == NULL)
		return (NULL);
	skip_quotes(str, trimmed_str);
	ft_free(&str);
	return (trimmed_str);
}

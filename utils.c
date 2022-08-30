/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:49:04 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/30 16:38:41 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_to_stderr(const char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
}

// This is ft_strjoin but with an additional argument
// to specify which input strings to free
char	*strjoin_free(char *s1, char *s2, int f)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	if (f == 1)
		ft_free(&s1);
	if (f == 2)
		ft_free(&s2);
	if (f == 3)
	{
		ft_free(&s1);
		ft_free(&s2);
	}
	return (joined);
}

char	*substr_free(char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	substr_length;
	size_t	i;

	if (start > ft_strlen(s))
		len = 0;
	else
	{
		substr_length = ft_strlen(&s[start]);
		if (len > substr_length)
			len = substr_length;
	}
	substr = malloc(sizeof(char) * (len + 1));
	if (substr == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = s[i + start];
		i++;
	}
	substr[i] = '\0';
	ft_free(&s);
	return (substr);
}

char	**copy_str_arr(char **arr)
{
	size_t	len;
	size_t	i;
	char	**arr_cpy;

	len = 0;
	while (arr[len] != NULL)
		len++;
	arr_cpy = ft_calloc(len + 1, sizeof(char *));
	if (arr_cpy == NULL)
	{
		// ! Handle error here
	}
	i = 0;
	while (i < len)
	{
		arr_cpy[i] = ft_strdup(arr[i]);
		if (arr_cpy[i] == NULL)
		{
			// ! Handle error here
		}
		i++;
	}
	arr_cpy[i] = NULL;
	return (arr_cpy);
}
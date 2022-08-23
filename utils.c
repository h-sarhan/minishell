/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:49:04 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 09:40:18 by hsarhan          ###   ########.fr       */
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

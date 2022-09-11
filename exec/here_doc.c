/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 14:52:15 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/11 15:00:00 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*resize(char **arr, int *old_len, char *limiter)
{
	int		i;
	char	*new_arr;

	i = 0;
	new_arr = ft_calloc(*old_len * 2 + 1, sizeof(char));
	// if (new_arr == NULL)
	// {
	// 	ft_free(arr);
	// 	ft_free(&limiter);
	// 	exit(1);
	// }
	while (i < *old_len)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	free(*arr);
	*old_len *= 2;
	return (new_arr);
}

/**
 * @brief Function reads from standard input and returns the content.
 * But the here doc fails when the limiter is the first line.
 * 
 * @param limiter 
 * @return char* 
 */
char	*read_from_stdin(char *limiter)
{
	char	*buff;
	int		buffer_len;
	int		i;
	char	ch;

	buffer_len = 100;
	buff = ft_calloc(buffer_len + 1, sizeof(char));
	if (buff == NULL)
		return (NULL);
	i = 0;
	while (ft_strnstr(buff, limiter, ft_strlen(buff)) == NULL)
	{
		if (i == buffer_len)
			buff = resize(&buff, &buffer_len, limiter);
		if (read(0, &ch, 1) < 1)
			break ;
		buff[i] = ch;
		i++;
	}
	buff[i] = '\0';
	if (ft_strnstr(buff, limiter, ft_strlen(buff)) != NULL)
		ft_strnstr(buff, limiter, ft_strlen(buff))[1] = '\0';
	return (buff);
}




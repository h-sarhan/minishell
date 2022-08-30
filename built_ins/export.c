/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 21:25:48 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/30 23:39:50 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**resize_str_arr(char **old, size_t new_size)
{
	size_t	i;
	char	**new_arr;

	new_arr = ft_calloc(new_size + 1, sizeof(char *));
	if (new_arr == NULL)
	{
		// ! Handle error here
	}
	i = 0;
	while (old[i] != NULL)
	{
		new_arr = old[i];
		i++;
	}
	return (new_arr);
}

void	ft_export(t_shell *shell, const char *name, const char *value)
{
	
}
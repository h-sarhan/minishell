/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 14:29:28 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_env_var(t_shell *shell, char **env_copy, char *to_look)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (shell->env[i] != NULL)
	{
		if (ft_strncmp(shell->env[i], to_look, ft_strlen(to_look)) != 0)
			env_copy[j++] = shell->env[i++];
		else
			ft_free(&shell->env[i++]);
	}
}

void	remove_declared_env_var(t_shell *shell, char **env_copy, char *to_look)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (shell->declared_env[i] != NULL)
	{
		if (ft_strncmp(shell->declared_env[i], to_look,
				ft_strlen(to_look)) != 0)
			env_copy[j++] = shell->declared_env[i++];
		else
			ft_free(&shell->declared_env[i++]);
	}
}

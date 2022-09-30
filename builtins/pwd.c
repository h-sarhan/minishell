/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 21:56:29 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Gets the path from env to current directory
 * and displays it.
 * 
 * @param step 
 * @param shell 
 */
void	ft_pwd(t_exec_step *step, t_shell *shell)
{
	char	*dir;

	dir = getcwd(NULL, 0);
	if (!dir)
		dir = get_env(shell, "PWD");
	if (!dir)
		return ;
	printf("%s\n", dir);
	ft_free(&dir);
	step->exit_code = 0;
	shell->last_exit_code = step->exit_code;
}

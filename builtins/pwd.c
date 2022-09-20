/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 21:56:29 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/20 19:36:17 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(t_exec_step *step, t_shell *shell)
{
	(void)step;
	char	*dir;
	
	dir = getcwd(NULL, 0);
	if (!dir)
		dir = get_env(shell, "PWD");
	// Doesnt really replicate bash but whatever
	if (!dir)
		return ;
	printf("%s\n", dir);
	ft_free(&dir);
	step->exit_code = 0;
	shell->last_exit_code = step->exit_code;
}

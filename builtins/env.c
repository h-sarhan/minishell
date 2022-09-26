/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 17:43:57 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/26 12:04:56 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Iterate through the copy of env variables array,
 * and print them out when env is called.
 * 
 * @param shell 
 * @param step 
 */
void	ft_env(t_shell *shell, t_exec_step *step)
{
	size_t	i;

	i = 0;
	if (step->cmd->arg_arr[1] != NULL)
	{
		step->exit_code = 1;
		shell->last_exit_code = step->exit_code;
		return ;
	}
	while (shell->env[i] != NULL)
	{
		printf("%s\n", shell->env[i]);
		i++;
	}
	step->exit_code = 0;
	shell->last_exit_code = step->exit_code;
}

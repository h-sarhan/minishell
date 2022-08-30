/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 13:47:43 by mkhan             #+#    #+#             */
/*   Updated: 2022/08/30 14:10:58 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void 	is_builtin(t_exec_step *step)
{
	if (ft_strcmp(step->cmd->arg_arr[0], "echo") == 0)
		ft_echo(step->content);
	if (ft_strcmp(step->cmd->arg_arr[0], "cd") == 0)
		ft_cd(step->content, mini->env);
	if (ft_strcmp(step->cmd->arg_arr[0], "pwd") == 0)
		ft_pwd(step->content);
	// if (ft_strcmp(step->cmd->arg_arr[0], "env") == 0)
	// 	ft_env();
	// if (ft_strcmp(step->cmd->arg_arr[0], "export") == 0)
	// 	ft_export();
	// if (ft_strcmp(step->cmd->arg_arr[0], "unset") == 0)
	// 	ft_unset();
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 14:54:15 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_invalid_path(t_exec_step *step)
{
	return (step->cmd->arg_arr[0] && access(step->cmd->arg_arr[0], X_OK) != -1
		&& !ft_strchr(step->cmd->arg_arr[0], '/'));
}

bool	check_invalid_command(t_exec_step *step, bool valid_redirs)
{
	return (step->cmd->arg_arr[0] && ((access(step->cmd->arg_arr[0], X_OK) == -1
				&& !is_builtin(step)) || is_dir(step->cmd->arg_arr[0])
			|| !valid_redirs));
}

bool	cmd_not_found_check(t_exec_step *step, bool valid_redirs)
{
	bool	check;

	check = (access(step->cmd->arg_arr[0], F_OK) == -1 && !is_builtin(step));
	if ((check || is_dir(step->cmd->arg_arr[0])) && valid_redirs
		&& !ft_strchr(step->cmd->arg_arr[0], '/'))
		return (true);
	if (access(step->cmd->arg_arr[0], F_OK) != -1
		&& access(step->cmd->arg_arr[0], X_OK) == -1
		&& !ft_strchr(step->cmd->arg_arr[0], '/') && valid_redirs)
		return (true);
	return (false);
}

bool	file_not_found_check(t_exec_step *step, bool valid_redirs)
{
	return ((access(step->cmd->arg_arr[0], F_OK) == -1 && !is_builtin(step))
		&& valid_redirs && ft_strchr(step->cmd->arg_arr[0], '/'));
}

bool	permission_denied_check(t_exec_step *step, bool valid_redirs)
{
	return ((access(step->cmd->arg_arr[0], X_OK) == -1
			&& !is_builtin(step)) && valid_redirs);
}

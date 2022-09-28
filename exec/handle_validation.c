/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 15:41:51 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/28 16:59:59 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	handle_invalid_path(t_shell *shell, t_exec_step *step,
	t_exec_flags *flags)
{
	ft_stderr("minishell: %s: command not found\n",
		step->cmd->arg_arr[0]);
	flags->exit = true;
	step->exit_code = 127;
	shell->last_exit_code = step->exit_code;
	ft_close(&shell->fd[0]);
	shell->fd[0] = open("/dev/null", O_RDONLY);
	if (!flags->first_flag)
		flags->first_flag = true;
	if (step->and_next || step->or_next)
		return (false);
	return (true);
}

bool	handle_invalid_cmd(t_shell *shell, t_exec_step *step,
	t_exec_flags *flags)
{
	if (cmd_not_found_check(step, flags->valid_redirs))
		cmd_not_found(shell, step, &flags->exit);
	else if (is_dir(step->cmd->arg_arr[0]) && flags->valid_redirs)
		cmd_is_dir(shell, step, &flags->exit);
	else if (file_not_found_check(step, flags->valid_redirs))
		file_not_found(shell, step, &flags->exit);
	else if (permission_denied_check(step, flags->valid_redirs))
		permission_denied(shell, step, &flags->exit);
	ft_close(&shell->fd[0]);
	shell->fd[0] = open("/dev/null", O_RDONLY);
	if (step->and_next || step->or_next)
		return (false);
	return (true);
}

void	check_command(t_shell *shell, t_list **steps, t_exec_step *step,
	t_exec_flags *flags)
{
	if (check_invalid_path(step) == true)
	{
		if (!handle_invalid_path(shell, step, flags))
		{
			flags->action = BREAK;
			return ;
		}
		*steps = (*steps)->next;
		flags->action = CONT;
		return ;
	}
	if (check_invalid_command(step, flags->valid_redirs) == true)
	{
		if (!flags->first_flag)
			flags->first_flag = true;
		if (!handle_invalid_cmd(shell, step, flags))
		{
			flags->action = BREAK;
			return ;
		}
		*steps = (*steps)->next;
		flags->action = CONT;
		return ;
	}
	flags->action = PASS;
}

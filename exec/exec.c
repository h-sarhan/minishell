/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 08:49:50 by hsarhan           #+#    #+#             */
/*   Updated: 2022/10/05 12:15:10 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_cmd_path(t_shell *shell, t_exec_step *step)
{
	char	*cmd_copy;

	if (step->cmd->arg_arr[0] && step->cmd->arg_arr[0][0] != '\0'
		&& access(step->cmd->arg_arr[0], X_OK) == -1
		&& !is_builtin(step) && !is_dir(step->cmd->arg_arr[0])
		&& ft_strchr(step->cmd->arg_arr[0], '/') == NULL )
	{
		cmd_copy = get_full_path(step->cmd->arg_arr[0], shell->env);
		if (cmd_copy != NULL)
			step->cmd->arg_arr[0] = cmd_copy;
	}
}

t_list	*reset_to_step(t_list *exec_steps, int step_number_start)
{
	t_list	*steps;
	int		wait_idx;

	steps = exec_steps;
	wait_idx = 0;
	while (steps && wait_idx < step_number_start)
	{
		steps = steps->next;
		wait_idx++;
	}
	return (steps);
}

void	init_exec_cmds(t_shell *shell, int *out_fd, t_exec_flags *flags,
	int step_number)
{
	shell->fd[0] = -1;
	shell->fd[1] = -1;
	*out_fd = -1;
	flags->w_status = 0;
	flags->first_flag = false;
	flags->exit = false;
	flags->step_num = step_number;
}

t_list	*go_to_step(t_exec_flags *flags, t_list *exec_steps,
	t_exec_step **step)
{
	t_list	*steps;

	flags->step_num_start = 0;
	steps = exec_steps;
	while (flags->step_num_start < flags->step_num && steps != NULL)
	{
		steps = steps->next;
		flags->step_num_start += 1;
	}
	if (steps == NULL)
		return (NULL);
	*step = steps->content;
	return (steps);
}

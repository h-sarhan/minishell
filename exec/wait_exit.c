/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 14:59:47 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/28 20:37:24 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*wait_cmds(t_list *steps, t_exec_flags *flags)
{
	t_exec_step	*step;

	while (steps && flags->step_num_start < flags->step_num)
	{
		step = steps->content;
		if (step->cmd->arg_arr[0] && !is_dir(step->cmd->arg_arr[0])
			&& (access(step->cmd->arg_arr[0], X_OK) != -1
				|| is_builtin(step)))
		{
			waitpid(step->cmd->pid, &flags->w_status, 0);
		}
		if (step->and_next || step->or_next)
			return (steps);
		flags->step_num_start++;
		steps = steps->next;
	}
	return (steps);
}

int	get_exit(t_list *exec_steps, t_exec_step *step, t_exec_flags *flags)
{
	if (!flags->exit && !WIFEXITED(flags->w_status)
		&& WIFSIGNALED(flags->w_status))
	{
		if (WTERMSIG(flags->w_status) == SIGINT)
			step->exit_code = 130;
		if (WTERMSIG(flags->w_status) == SIGQUIT)
		{
			printf("Quit\n");
			step->exit_code = 131;
		}
		return (step->exit_code);
	}
	if (!flags->exit)
	{
		if (!(((t_exec_step *)exec_steps->content)->pipe_next == false
				&& parent_builtin(exec_steps->content)))
		{
			step->exit_code = WEXITSTATUS(flags->w_status);
		}
	}
	return (step->exit_code);
}

t_list	*wait_and_get_exit(t_shell *shell, t_exec_step *step,
	t_list *exec_steps, t_exec_flags *flags)
{
	t_list	*steps;
	int		i;

	if (step->cmd)
	{
		steps = reset_to_step(exec_steps, flags->step_num_start);
		steps = wait_cmds(steps, flags);
		shell->last_exit_code = get_exit(exec_steps, step, flags);
	}
	else
	{
		i = 0;
		steps = exec_steps;
		while (steps && i < flags->step_num)
		{
			steps = steps->next;
			i++;
		}
	}
	return (steps);
}

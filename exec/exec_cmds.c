/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 17:10:10 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/29 23:04:00 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	run_cmds(t_shell *shell, t_exec_step *step, t_exec_flags *flags,
	int out_fd)
{
	if (!flags->first_flag && flags->valid_redirs)
	{
		shell->fd = first_cmd(step, shell->fd, shell, out_fd);
		flags->first_flag = true;
	}
	else if (flags->valid_redirs)
		shell->fd = mid_cmd(step, shell->fd, shell, out_fd);
	if (step->and_next || step->or_next)
		return (false);
	return (true);
}

static t_exec_step	*run_exec_cmds(t_shell *shell, t_list **steps, int *out_fd,
	t_exec_flags *flags)
{
	t_exec_step	*step;

	flags->action = PASS;
	step = (*steps)->content;
	if (step->subexpr_line != NULL)
	{
		if (exec_subexpr(shell, step, flags, steps) == false)
		{
			flags->action = BREAK;
			return (step);
		}
		flags->action = CONT;
		return (step);
	}
	flags->exit = false;
	flags->valid_redirs = open_redirs(shell, step, &flags->exit, out_fd);
	set_cmd_path(shell, step);
	check_command(shell, steps, step, flags);
	if (flags->action == BREAK)
		return (step);
	else if (flags->action == CONT)
	{
		ft_close(out_fd);
		return (step);
	}
	if (run_cmds(shell, step, flags, *out_fd) == false)
		flags->action = BREAK;
	return (step);
}

void	exec_cmds(t_shell *shell, t_list *exec_steps, int step_number,
	char *current_line)
{
	t_exec_step		*step;
	t_list			*steps;
	t_exec_flags	flags;
	int				out_fd;

	init_exec_cmds(shell, &out_fd, &flags, step_number);
	steps = go_to_step(&flags, exec_steps, &step);
	if (steps == NULL)
		return ;
	while (steps)
	{
		flags.step_num++;
		step = run_exec_cmds(shell, &steps, &out_fd, &flags);
		if (flags.action == CONT)
			continue ;
		if (flags.action == BREAK)
			break ;
		steps = steps->next;
	}
	ft_close(&shell->fd[0]);
	ft_close(&out_fd);
	steps = wait_and_get_exit(shell, step, exec_steps, &flags);
	shell->current_line = current_line;
	handle_and_or(shell, step, flags.step_num, &steps);
}

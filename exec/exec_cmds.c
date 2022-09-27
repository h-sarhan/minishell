/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 08:49:50 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/27 18:56:33 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	exec_subexpr(t_shell *shell, t_exec_step *step, bool *first_flag,
	t_list **steps)
{
	t_list		*sub_tokens;
	t_list		*sub_steps;
	bool		success;

	sub_tokens = tokenize_line(shell, step->subexpr_line, &success);
	sub_steps = parse_tokens(sub_tokens, &success);
	ft_lstclear(&sub_tokens, free_token);
	ft_lstadd_back(&shell->steps_to_free, ft_lstnew(sub_steps));
	exec_cmds(shell, sub_steps, 0, step->subexpr_line);
	if (!(*first_flag))
		*first_flag = true;
	if (step->and_next || step->or_next)
		return (false);
	*steps = (*steps)->next;
	return (true);
}

bool	open_redirs(t_shell *shell, t_exec_step *step,
	bool *exit_flag, int *out_fd)
{
	bool	valid_redirs;

	valid_redirs = check_valid_redir(step);
	if (valid_redirs == false)
	{
		*exit_flag = true;
		step->exit_code = 1;
		shell->last_exit_code = step->exit_code;
	}
	*out_fd = exec_outredir(step);
	if (*out_fd == -2)
	{
		valid_redirs = false;
		*exit_flag = true;
		step->exit_code = 1;
		*out_fd = -1;
		shell->last_exit_code = step->exit_code;
	}
	return (valid_redirs);
}

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

void	cmd_not_found(t_shell *shell, t_exec_step *step, bool *exit_flag)
{
	ft_stderr("minishell: %s: command not found\n", step->cmd->arg_arr[0]);
	*exit_flag = true;
	step->exit_code = 127;
	shell->last_exit_code = step->exit_code;
}

void	cmd_is_dir(t_shell *shell, t_exec_step *step, bool *exit_flag)
{
	ft_stderr("minishell: %s: is a directory\n", step->cmd->arg_arr[0]);
	*exit_flag = true;
	step->exit_code = 126;
	shell->last_exit_code = step->exit_code;
}

bool	file_not_found_check(t_exec_step *step, bool valid_redirs)
{
	return ((access(step->cmd->arg_arr[0], F_OK) == -1 && !is_builtin(step))
		&& valid_redirs && ft_strchr(step->cmd->arg_arr[0], '/'));
}

void	file_not_found(t_shell *shell, t_exec_step *step, bool *exit_flag)
{
	ft_stderr("minishell: %s: No such file or directory\n",
		step->cmd->arg_arr[0]);
	*exit_flag = true;
	step->exit_code = 127;
	shell->last_exit_code = step->exit_code;
}

bool	permission_denied_check(t_exec_step *step, bool valid_redirs)
{
	return ((access(step->cmd->arg_arr[0], X_OK) == -1
			&& !is_builtin(step)) && valid_redirs);
}

void	permission_denied(t_shell *shell, t_exec_step *step, bool *exit_flag)
{
	ft_stderr("minishell: %s: Permission denied\n", step->cmd->arg_arr[0]);
	*exit_flag = true;
	step->exit_code = 126;
	shell->last_exit_code = step->exit_code;
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

int	handle_and_next(t_shell *shell, t_list **steps, t_exec_step *step,
	int step_number)
{
	if (shell->last_exit_code != 0)
	{
		while (*steps != NULL && step->and_next)
		{
			step = (*steps)->content;
			*steps = (*steps)->next;
			step_number++;
		}
		if (step->or_next)
			step_number--;
		while (*steps && !step->and_next && !step->or_next)
		{
			step = (*steps)->content;
			*steps = (*steps)->next;
			step_number++;
		}
		if (*steps == NULL)
			return (-1);
	}
	return (step_number);
}

int	handle_or_next(t_shell *shell, t_list **steps, t_exec_step *step,
	int step_number)
{
	if (shell->last_exit_code == 0)
	{
		while (*steps != NULL && step->or_next)
		{
			step = (*steps)->content;
			*steps = (*steps)->next;
			step_number++;
		}
		if (step->and_next)
		{
			step_number--;
		}
		while (*steps && !step->and_next && !step->or_next)
		{
			step = (*steps)->content;
			*steps = (*steps)->next;
			step_number++;
		}
		if (*steps == NULL)
			return (-1);
	}
	return (step_number);
}

void	reparse(t_shell *shell, char *current_line, int step_number)
{
	t_list		*tokens;
	t_list		*new_steps;
	bool		success;

	ft_lstclear(&shell->tokens, free_token);
	tokens = tokenize_line(shell, current_line, &success);
	new_steps = parse_tokens(tokens, &success);
	ft_lstadd_back(&shell->steps_to_free, ft_lstnew(new_steps));
	shell->tokens = tokens;
	shell->steps = new_steps;
	exec_cmds(shell, new_steps, step_number, current_line);
}

void	handle_and_or(t_shell *shell, t_exec_step *step, int step_number,
	t_list *steps)
{
	if (step && step->and_next)
		step_number = handle_and_next(shell, &steps, step, step_number);
	else if (step && step->or_next)
		step_number = handle_or_next(shell, &steps, step, step_number);
	if (step && steps != NULL && (step->and_next || step->or_next))
		reparse(shell, shell->current_line, step_number);
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

bool	run_cmds(t_shell *shell, t_exec_step *step, t_exec_flags *flags,
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

t_exec_step	*run_exec_cmds(t_shell *shell, t_list **steps, int *out_fd,
	t_exec_flags *flags)
{
	t_exec_step	*step;

	flags->action = PASS;
	step = (*steps)->content;
	if (step->subexpr_line != NULL)
	{
		if (exec_subexpr(shell, step, &flags->first_flag, steps) == false)
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
		return (step);
	if (run_cmds(shell, step, flags, *out_fd) == false)
		flags->action = BREAK;
	return (step);
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
	handle_and_or(shell, step, flags.step_num, steps);
}

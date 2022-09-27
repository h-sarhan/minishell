/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 08:49:50 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/27 16:52:57 by hsarhan          ###   ########.fr       */
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

bool	check_and_open_redirs(t_shell *shell, t_exec_step *step,
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

bool	handle_invalid_path(t_shell *shell, t_exec_step *step, bool *exit_flag,
	bool *flag)
{
	ft_stderr("minishell: %s: command not found\n",
		step->cmd->arg_arr[0]);
	*exit_flag = true;
	step->exit_code = 127;
	shell->last_exit_code = step->exit_code;
	ft_close(&shell->fd[0]);
	shell->fd[0] = open("/dev/null", O_RDONLY);
	if (!*flag)
		*flag = true;
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

bool	handle_invalid_cmd(t_shell *shell, t_exec_step *step, bool valid_redirs,
	bool *exit_flag)
{
	if (cmd_not_found_check(step, valid_redirs))
		cmd_not_found(shell, step, exit_flag);
	else if (is_dir(step->cmd->arg_arr[0]) && valid_redirs)
		cmd_is_dir(shell, step, exit_flag);
	else if (file_not_found_check(step, valid_redirs))
		file_not_found(shell, step, exit_flag);
	else if (permission_denied_check(step, valid_redirs))
		permission_denied(shell, step, exit_flag);
	ft_close(&shell->fd[0]);
	shell->fd[0] = open("/dev/null", O_RDONLY);
	if (step->and_next || step->or_next)
		return (false);
	return (true);
}

t_list	*skip_to_step(t_list *exec_steps, int step_number_start)
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

t_list	*wait_cmds(t_list *steps, int step_number_start, int step_number,
	int *w_status)
{
	t_exec_step	*step;

	while (steps && step_number_start < step_number)
	{
		step = steps->content;
		if (step->cmd->arg_arr[0] && !is_dir(step->cmd->arg_arr[0])
			&& (access(step->cmd->arg_arr[0], X_OK) != -1
				|| is_builtin(step)))
		{
			waitpid(step->cmd->pid, w_status, 0);
		}
		if (step->and_next || step->or_next)
			return (steps);
		step_number_start++;
		steps = steps->next;
	}
	return (steps);
}

int	get_exit(t_list *exec_steps, t_exec_step *step, bool exit_flag,
	int w_status)
{
	if (!exit_flag && !WIFEXITED(w_status) && WIFSIGNALED(w_status))
	{
		if (WTERMSIG(w_status) == SIGINT)
			step->exit_code = 130;
		if (WTERMSIG(w_status) == SIGQUIT)
		{
			printf("Quit\n");
			step->exit_code = 131;
		}
	}
	if (!exit_flag)
	{
		if (!(((t_exec_step *)exec_steps->content)->pipe_next == false
				&& parent_builtin(exec_steps->content)))
		{
			step->exit_code = WEXITSTATUS(w_status);
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

void	exec_cmds(t_shell *shell, t_list *exec_steps, int step_number,
	char *current_line)
{
	t_exec_step	*step;
	t_list		*steps;
	bool		first_flag;
	bool		exit_flag;
	bool		valid_redirs;
	int			out_fd;
	int			w_status;
	int			step_number_start;

	shell->fd[0] = -1;
	shell->fd[1] = -1;
	out_fd = -1;
	w_status = 0;
	step_number_start = 0;
	steps = exec_steps;
	while (step_number_start < step_number && steps != NULL)
	{
		steps = steps->next;
		step_number_start++;
	}
	if (steps == NULL)
		return ;
	step = steps->content;
	first_flag = false;
	exit_flag = false;
	while (steps)
	{
		step_number++;
		step = steps->content;
		if (step->subexpr_line != NULL)
		{
			if (exec_subexpr(shell, step, &first_flag, &steps) == false)
				break ;
			continue ;
		}
		exit_flag = false;
		valid_redirs = check_and_open_redirs(shell, step, &exit_flag, &out_fd);
		set_cmd_path(shell, step);
		if (check_invalid_path(step) == true)
		{
			if (!handle_invalid_path(shell, step, &exit_flag, &first_flag))
				break ;
			steps = steps->next;
			continue ;
		}
		if (check_invalid_command(step, valid_redirs) == true)
		{
			if (!first_flag)
				first_flag = true;
			if (!handle_invalid_cmd(shell, step, valid_redirs, &exit_flag))
				break ;
			steps = steps->next;
			continue ;
		}
		if (!first_flag && valid_redirs)
		{
			shell->fd = first_cmd(step, shell->fd, shell, out_fd);
			first_flag = true;
		}
		else if (valid_redirs)
			shell->fd = mid_cmd(step, shell->fd, shell, out_fd);
		if (step->and_next || step->or_next)
			break ;
		steps = steps->next;
	}
	ft_close(&shell->fd[0]);
	ft_close(&out_fd);
	if (step->cmd)
	{
		steps = skip_to_step(exec_steps, step_number_start);
		steps = wait_cmds(steps, step_number_start, step_number, &w_status);
		shell->last_exit_code = get_exit(exec_steps, step, exit_flag, w_status);
	}
	shell->current_line = current_line;
	handle_and_or(shell, step, step_number, steps);
}

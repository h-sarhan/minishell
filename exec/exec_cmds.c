/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 08:49:50 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/27 10:04:01 by hsarhan          ###   ########.fr       */
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

void	exec_cmds(t_shell *shell, t_list *exec_steps, int step_number,
	char *current_line)
{
	t_exec_step	*step;
	t_list		*steps;
	t_list		*tokens;
	t_list		*new_steps;
	bool		first_flag;
	bool		exit_flag;
	bool		success;
	bool		valid_redirs;
	int			out_fd;
	int			w_status;
	int			i;
	int			wait_idx;

	shell->fd[0] = -1;
	shell->fd[1] = -1;
	out_fd = -1;
	w_status = 0;
	i = 0;
	steps = exec_steps;
	while (i < step_number && steps != NULL)
	{
		steps = steps->next;
		i++;
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
		if (check_invalid_path(step))
		{
			if (!handle_invalid_path(shell, step, &exit_flag, &first_flag))
				break ;
			steps = steps->next;
			continue ;
		}
		if (check_invalid_command(step, valid_redirs) == true)
		{
			if (((access(step->cmd->arg_arr[0], F_OK) == -1
						&& !is_builtin(step)) || is_dir(step->cmd->arg_arr[0]))
				&& valid_redirs && !ft_strchr(step->cmd->arg_arr[0], '/'))
			{
				ft_stderr("minishell: %s: command not found\n",
					step->cmd->arg_arr[0]);
				exit_flag = true;
				step->exit_code = 127;
				shell->last_exit_code = step->exit_code;
			}
			else if (access(step->cmd->arg_arr[0], F_OK) != -1
				&& access(step->cmd->arg_arr[0], X_OK) == -1
				&& !ft_strchr(step->cmd->arg_arr[0], '/') && valid_redirs)
			{
				ft_stderr("minishell: %s: command not found\n",
					step->cmd->arg_arr[0]);
				exit_flag = true;
				step->exit_code = 127;
				shell->last_exit_code = step->exit_code;
			}
			else if (is_dir(step->cmd->arg_arr[0]) && valid_redirs)
			{
				ft_stderr("minishell: %s: is a directory\n",
					step->cmd->arg_arr[0]);
				exit_flag = true;
				step->exit_code = 126;
				shell->last_exit_code = step->exit_code;
			}
			else if ((access(step->cmd->arg_arr[0], F_OK) == -1
					&& !is_builtin(step)) && valid_redirs
				&& ft_strchr(step->cmd->arg_arr[0], '/'))
			{
				ft_stderr("minishell: %s: No such file or directory\n",
					step->cmd->arg_arr[0]);
				exit_flag = true;
				step->exit_code = 127;
				shell->last_exit_code = step->exit_code;
			}
			else if ((access(step->cmd->arg_arr[0], X_OK) == -1
					&& !is_builtin(step)) && valid_redirs)
			{
				ft_stderr("minishell: %s: Permission denied\n",
					step->cmd->arg_arr[0]);
				exit_flag = true;
				step->exit_code = 126;
				shell->last_exit_code = step->exit_code;
			}
			ft_close(&shell->fd[0]);
			shell->fd[0] = open("/dev/null", O_RDONLY);
			if (!first_flag)
				first_flag = true;
			if (step->and_next || step->or_next)
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
	// ! Do not wait for subexpressions
	if (step->cmd)
	{
		steps = exec_steps;
		wait_idx = 0;
		while (steps && wait_idx < i)
		{
			steps = steps->next;
			wait_idx++;
		}
		while (steps && i < step_number)
		{
			step = steps->content;
			if (step->cmd->arg_arr[0] && !is_dir(step->cmd->arg_arr[0])
				&& (access(step->cmd->arg_arr[0], X_OK) != -1
					|| is_builtin(step)))
			{
				waitpid(step->cmd->pid, &w_status, 0);
			}
			if (step->and_next || step->or_next)
				break ;
			i++;
			steps = steps->next;
		}
		if (!exit_flag && !WIFEXITED(w_status) && WIFSIGNALED(w_status))
		{
			if (WTERMSIG(w_status) == SIGINT)
			{
				step->exit_code = 130;
				shell->last_exit_code = step->exit_code;
			}
			if (WTERMSIG(w_status) == SIGQUIT)
			{
				printf("Quit\n");
				step->exit_code = 131;
				shell->last_exit_code = step->exit_code;
			}
			return ;
		}
		if (!exit_flag)
		{
			// Checking the only case where we dont fork
			if (!(((t_exec_step *)exec_steps->content)->pipe_next == false
					&& parent_builtin(exec_steps->content)))
			{
				step->exit_code = WEXITSTATUS(w_status);
				shell->last_exit_code = step->exit_code;
			}
		}
	}
	if (step == NULL)
		return ;
	if (step->and_next)
	{
		if (shell->last_exit_code != 0)
		{
			while (steps != NULL && step->and_next)
			{
				step = steps->content;
				steps = steps->next;
				step_number++;
			}
			if (step->or_next)
				step_number--;
			while (steps && (!step->and_next && !step->or_next))
			{
				step = steps->content;
				steps = steps->next;
				step_number++;
			}
			if (steps == NULL)
				return ;
		}
		ft_lstclear(&shell->tokens, free_token);
		tokens = tokenize_line(shell, current_line, &success);
		new_steps = parse_tokens(tokens, &success);
		ft_lstadd_back(&shell->steps_to_free, ft_lstnew(new_steps));
		shell->tokens = tokens;
		shell->steps = new_steps;
		exec_cmds(shell, new_steps, step_number, current_line);
	}
	else if (step->or_next)
	{
		if (shell->last_exit_code == 0)
		{
			while (steps != NULL && step->or_next)
			{
				step = steps->content;
				steps = steps->next;
				step_number++;
			}
			if (step->and_next)
			{
				step_number--;
			}
			while (steps && (!step->and_next && !step->or_next))
			{
				step = steps->content;
				steps = steps->next;
				step_number++;
			}
			if (steps == NULL)
				return ;
		}
		ft_lstclear(&shell->tokens, free_token);
		tokens = tokenize_line(shell, current_line, &success);
		new_steps = parse_tokens(tokens, &success);
		ft_lstadd_back(&shell->steps_to_free, ft_lstnew(new_steps));
		shell->tokens = tokens;
		shell->steps = new_steps;
		exec_cmds(shell, new_steps, step_number, current_line);
	}
}

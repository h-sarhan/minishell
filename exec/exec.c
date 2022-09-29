/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 08:49:50 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/29 20:59:39 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_heredocs(t_list *substeps)
{
	int			num_heredocs;
	t_exec_step	*step;
	t_redir		*last_redir;

	num_heredocs = 0;
	while (substeps != NULL)
	{
		step = substeps->content;
		if (step->cmd != NULL && step->cmd->redirs != NULL)
		{
			last_redir = last_inredir(step->cmd->redirs);
			if (last_redir != NULL)
				num_heredocs++;
		}
		substeps = substeps->next;
	}
	return (num_heredocs);
}

void	skip_sub_heredocs(t_list *heredocs, int num_skipped)
{
	int	i;

	i = 0;
	if (num_skipped == 0)
		return ;
	while (heredocs != NULL && heredocs->content == NULL)
		heredocs = heredocs->next;
	if (heredocs == NULL)
		return ;
	while (i < num_skipped)
	{
		ft_free(&heredocs->content);
		heredocs->content = NULL;
		heredocs = heredocs->next;
		i++;
	}
}

bool	exec_subexpr(t_shell *shell, t_exec_step *step, t_exec_flags *flags,
	t_list **steps)
{
	t_list		*sub_tokens;
	t_list		*sub_steps;
	bool		success;
	int			pid;
	int			heredocs_to_skip;

	sub_tokens = tokenize_line(shell, step->subexpr_line, &success);
	sub_steps = parse_tokens(sub_tokens, &success);
	ft_lstclear(&sub_tokens, free_token);
	ft_lstadd_back(&shell->steps_to_free, ft_lstnew(sub_steps));
	pid = fork();
	if (pid == 0)
	{
		exec_cmds(shell, sub_steps, 0, step->subexpr_line);
		ft_lstclear(&shell->tokens, free_token);
		free_steps(&shell->steps_to_free);
		ft_close(&g_dupstdin);
		free_split_array(shell->env);
		free_split_array(shell->declared_env);
		ft_lstclear(&shell->heredoc_contents, free);
		ft_free(&shell->fd);
		get_next_line(-1);
		exit(shell->last_exit_code);
	}
	waitpid(pid, &flags->w_status, 0);
	heredocs_to_skip = count_heredocs(sub_steps);
	skip_sub_heredocs(shell->heredoc_contents, heredocs_to_skip);
	step->exit_code = WEXITSTATUS(flags->w_status);
	shell->last_exit_code = step->exit_code;
	if (!(flags->first_flag))
		flags->first_flag = true;
	if (step->and_next || step->or_next)
		return (false);
	*steps = (*steps)->next;
	return (true);
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

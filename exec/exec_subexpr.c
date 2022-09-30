/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subexpr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 07:57:09 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/30 07:57:21 by hsarhan          ###   ########.fr       */
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

static int	parse_and_fork_subexpr(t_shell *shell, t_exec_step *step,
	t_list **sub_steps)
{
	t_list		*sub_tokens;
	int			pid;
	bool		success;

	sub_tokens = tokenize_line(shell, step->subexpr_line, &success);
	*sub_steps = parse_tokens(sub_tokens, &success);
	ft_lstclear(&sub_tokens, free_token);
	ft_lstadd_back(&shell->steps_to_free, ft_lstnew(*sub_steps));
	pid = fork();
	if (pid == 0)
	{
		exec_cmds(shell, *sub_steps, 0, step->subexpr_line);
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
	return (pid);
}

bool	exec_subexpr(t_shell *shell, t_exec_step *step, t_exec_flags *flags,
	t_list **steps)
{
	int			pid;
	t_list		*sub_steps;
	int			heredocs_to_skip;

	pid = parse_and_fork_subexpr(shell, step, &sub_steps);
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

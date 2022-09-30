/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 00:12:36 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	*run_parent_builtin(t_exec_step *step, t_shell *shell, int *fd)
{
	run_builtin(step, shell, false);
	if (ft_strcmp(step->cmd->arg_arr[0], "exit") == 0)
	{
		if (step->cmd->arg_arr)
		{
			ft_lstclear(&shell->tokens, free_token);
			free_steps(&shell->steps_to_free);
			free_split_array(shell->env);
			free_split_array(shell->declared_env);
			ft_lstclear(&shell->heredoc_contents, free);
			ft_close(&g_dupstdin);
			ft_free(&fd);
		}
		exit(shell->last_exit_code);
	}
	return (fd);
}

static void	dup_pipe_fds(t_redir *inredir, int *heredoc_fds, t_exec_step *step,
	int *fd)
{
	if (inredir && inredir->type == HEREDOC)
	{
		ft_close(&heredoc_fds[1]);
		dup2(heredoc_fds[0], 0);
	}
	if (step->pipe_next)
	{
		ft_close(&fd[0]);
		dup2(fd[1], 1);
	}
}

static void	dup_redir_fds(int in_fd, int out_fd)
{
	if (in_fd != -1)
		dup2(in_fd, 0);
	if (out_fd != -1)
		dup2(out_fd, 1);
}

int	*first_cmd(t_exec_step *step, int *fds, t_shell *shell, int out_fd)
{
	int			in_fd;
	int			heredoc_fds[2];
	int			exit_code;
	t_redir		*inredir;

	in_fd = cmd_init(shell, &inredir, step, heredoc_fds);
	pipe_fds(step, fds);
	if (parent_builtin(step) && !step->pipe_next)
		return (run_parent_builtin(step, shell, fds));
	if (step->cmd->arg_arr[0] != NULL)
		step->cmd->pid = fork();
	if (step->cmd->arg_arr[0] != NULL && step->cmd->pid == 0)
	{
		dup_pipe_fds(inredir, heredoc_fds, step, fds);
		dup_redir_fds(in_fd, out_fd);
		if (is_builtin(step))
		{
			exit_code = run_child_builtin(shell, step, fds, heredoc_fds);
			ft_close(&out_fd);
			ft_close(&in_fd);
			exit(exit_code);
		}
		execve(step->cmd->arg_arr[0], step->cmd->arg_arr, shell->env);
	}
	return (cmd_cleanup(fds, &in_fd, &out_fd, heredoc_fds));
}

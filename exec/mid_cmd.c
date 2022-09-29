/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mid_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 07:43:36 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/29 23:49:07 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	dup_heredoc_fds(t_redir *inredir, int *heredoc_fds, int *fdtmp)
{
	dup2(*fdtmp, 0);
	if (inredir && inredir->type == HEREDOC)
	{
		ft_close(&heredoc_fds[1]);
		ft_close(fdtmp);
		dup2(heredoc_fds[0], 0);
	}
}

static void	dup_pipes_and_redirs(t_exec_step *step, int *fds, int in_fd,
	int out_fd)
{
	if (step->pipe_next)
	{	
		ft_close(&fds[0]);
		dup2(fds[1], 1);
	}
	if (in_fd != -1)
		dup2(in_fd, 0);
	if (out_fd != -1)
		dup2(out_fd, 1);
}

static void	child_builtin_cleanup(int *out_fd, int *in_fd, int *fdtmp,
	int exit_code)
{
	ft_close(out_fd);
	ft_close(in_fd);
	ft_close(fdtmp);
	exit(exit_code);
}

static void	close_fds(const t_exec_step *step, int *fds, int *fdtmp)
{
	if (!step->pipe_next)
		ft_close(&fds[0]);
	ft_close(fdtmp);
}

int	*mid_cmd(t_exec_step *step, int *fds, t_shell *shell, int out_fd)
{
	int			in_fd;
	int			fdtmp;
	int			heredoc_fds[2];
	int			exit_code;
	t_redir		*inredir;

	fdtmp = fds[0];
	in_fd = cmd_init(shell, &inredir, step, heredoc_fds);
	pipe_fds(step, fds);
	if (step->cmd->arg_arr[0] != NULL)
		step->cmd->pid = fork();
	if (step->cmd->arg_arr[0] != NULL && step->cmd->pid == 0)
	{
		dup_heredoc_fds(inredir, heredoc_fds, &fdtmp);
		dup_pipes_and_redirs(step, fds, in_fd, out_fd);
		if (is_builtin(step))
		{
			exit_code = run_child_builtin(shell, step, fds, heredoc_fds);
			child_builtin_cleanup(&out_fd, &in_fd, &fdtmp, exit_code);
		}
		execve(step->cmd->arg_arr[0], step->cmd->arg_arr, shell->env);
	}
	close_fds(step, fds, &fdtmp);
	return (cmd_cleanup(fds, &in_fd, &out_fd, heredoc_fds));
}

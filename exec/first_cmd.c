/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 00:12:36 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/27 01:13:33 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	open_last_redir(t_exec_step *step, int *heredoc_fds,
	t_redir **inredir)
{
	int			in_fd;

	in_fd = -1;
	*inredir = last_inredir(step->cmd->redirs);
	if (*inredir != NULL)
	{
		if ((*inredir)->type == INPUT_REDIR)
			in_fd = open((*inredir)->file, O_RDONLY);
		else
		{
			pipe(heredoc_fds);
			ft_putstr_fd(step->cmd->heredoc_contents, heredoc_fds[1]);
		}
	}
	return (in_fd);
}

static int	*run_parent_builtin(t_exec_step *step, t_shell *shell, int *fd)
{
	int	exit_code;

	run_builtin(step, shell, false);
	if (ft_strcmp(step->cmd->arg_arr[0], "exit") == 0)
	{
		exit_code = step->exit_code;
		if (step->cmd->arg_arr)
		{
			ft_lstclear(&shell->tokens, free_token);
			free_steps(&shell->steps_to_free);
			free_split_array(shell->env);
			free_split_array(shell->declared_env);
			ft_free(&fd);
		}
		exit(exit_code);
	}
	return (fd);
}

void	dup_pipe_fds(t_redir *inredir, int *heredoc_fds, t_exec_step *step,
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

void	dup_redir_fds(int in_fd, int out_fd)
{
	if (in_fd != -1)
		dup2(in_fd, 0);
	if (out_fd != -1)
		dup2(out_fd, 1);
}

int	run_child_builtin(t_shell *shell, t_exec_step *step, int *fds,
	int *heredoc_fds)
{
	int	exit_code;

	run_builtin(step, shell, true);
	ft_close(&fds[1]);
	ft_close(&fds[0]);
	ft_close(&heredoc_fds[0]);
	ft_close(&heredoc_fds[1]);
	ft_close(&g_dupstdin);
	exit_code = step->exit_code;
	ft_lstclear(&shell->tokens, free_token);
	free_steps(&shell->steps_to_free);
	free_split_array(shell->env);
	free_split_array(shell->declared_env);
	ft_free(&fds);
	return (exit_code);
}

int	*first_cmd_cleanup(int *fds, int *in_fd, int *out_fd,
	int *heredoc_fds)
{
	ft_close(in_fd);
	ft_close(out_fd);
	ft_close(&heredoc_fds[0]);
	ft_close(&heredoc_fds[1]);
	ft_close(&fds[1]);
	return (fds);
}

int	first_cmd_init(t_redir **inredir, t_exec_step *step, int *heredoc_fds,
	int *fds)
{
	int	in_fd;

	*inredir = NULL;
	heredoc_fds[0] = -1;
	heredoc_fds[1] = -1;
	if (step->pipe_next)
		pipe(fds);
	in_fd = -1;
	if (step->cmd->redirs)
		in_fd = open_last_redir(step, heredoc_fds, inredir);
	return (in_fd);
}

int	*first_cmd(t_exec_step *step, int *fds, t_shell *shell, int out_fd)
{
	int			in_fd;
	int			heredoc_fds[2];
	int			exit_code;
	t_redir		*inredir;

	in_fd = first_cmd_init(&inredir, step, heredoc_fds, fds);
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
	return (first_cmd_cleanup(fds, &in_fd, &out_fd, heredoc_fds));
}

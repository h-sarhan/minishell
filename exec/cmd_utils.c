/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 08:12:47 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/27 08:17:14 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_last_redir(t_exec_step *step, int *heredoc_fds, t_redir **inredir)
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

int	*cmd_cleanup(int *fds, int *in_fd, int *out_fd,
	int *heredoc_fds)
{
	ft_close(in_fd);
	ft_close(out_fd);
	ft_close(&heredoc_fds[0]);
	ft_close(&heredoc_fds[1]);
	ft_close(&fds[1]);
	return (fds);
}

int	cmd_init(t_redir **inredir, t_exec_step *step, int *heredoc_fds,
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

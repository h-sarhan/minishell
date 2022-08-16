/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:02:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/07/04 12:03:42 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Runs the command to the left of the pipe.
// If the infile file descriptor is invalid then it will be replaced
// with the "/dev/null" file descriptor which will return the EOF character
// when it is read from.
void	run_first_cmd(t_cmd *cmd, int *pipe_fds, int *fds, t_list *cmds)
{
	int	null_fd;

	null_fd = open_file("/dev/null", 0);
	close_fd(pipe_fds[READ], cmds);
	close_fd(fds[1], cmds);
	if (fds[0] == -1)
	{
		dup_fd(null_fd, STDIN, cmds, NULL);
		close_fd(cmd->in_fd, cmds);
	}
	else
	{
		dup_fd(cmd->in_fd, STDIN, cmds, NULL);
		close_fd(null_fd, cmds);
	}
	dup_fd(cmd->out_fd, STDOUT, cmds, NULL);
	if (fds[0] != -1)
		execve(cmd->cmd_args[0], cmd->cmd_args, cmd->env);
	close_fd(pipe_fds[WRITE], cmds);
	close_fd(fds[0], cmds);
	close_fd(null_fd, cmds);
	close_fd(STDOUT, cmds);
	close_fd(STDIN, cmds);
	ft_lstclear(&cmds, free_cmd);
	exit(EXIT_FAILURE);
}

// Runs a command in the middle
void	run_middle_cmd(t_cmd *cmd, int *pipe_fds, int *fds, t_list *cmds)
{
	close_fd(fds[0], cmds);
	close_fd(fds[1], cmds);
	close_fd(pipe_fds[READ], cmds);
	dup_fd(cmd->in_fd, STDIN, cmds, NULL);
	dup_fd(cmd->out_fd, STDOUT, cmds, NULL);
	execve(cmd->cmd_args[0], cmd->cmd_args, cmd->env);
	close_fd(pipe_fds[WRITE], cmds);
	ft_lstclear(&cmds, free_cmd);
	exit(EXIT_FAILURE);
}

// Runs the last command
void	run_last_cmd(t_cmd *cmd, int *pipe_fds, int *fds, t_list *cmds)
{
	close_fd(fds[0], cmds);
	dup_fd(cmd->in_fd, STDIN, cmds, NULL);
	dup_fd(cmd->out_fd, STDOUT, cmds, NULL);
	if (fds[1] != -1)
		execve(cmd->cmd_args[0], cmd->cmd_args, cmd->env);
	close_fd(pipe_fds[READ], cmds);
	close_fd(fds[1], cmds);
	close_fd(STDIN, cmds);
	ft_lstclear(&cmds, free_cmd);
	exit(EXIT_FAILURE);
}

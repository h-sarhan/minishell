/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:16:54 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/28 16:59:35 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_dupstdin;

/**
 * @brief Traverse through the list of input redirections,
 * and return the last input redirection.
 * 
 * @param in_redir 
 * @return t_redir* 
 */
t_redir	*last_inredir(t_list *in_redir)
{
	t_redir	*last;
	t_redir	*current_redir;

	last = NULL;
	while (in_redir)
	{
		current_redir = in_redir->content;
		if (current_redir->type == INPUT_REDIR
			|| current_redir->type == HEREDOC)
			last = current_redir;
		in_redir = in_redir->next;
	}
	return (last);
}

/**
 * @brief Create a redir file if the given file doesn't exist.
 * 
 * @param redir_file 
 * @param out_fd 
 * @return true 
 * @return false 
 */
bool	create_redir_file(t_redir *redir_file, int *out_fd)
{
	ft_close(out_fd);
	if (redir_file->type == APPEND)
		*out_fd = open(redir_file->file,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*out_fd = open(redir_file->file,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*out_fd == -1)
	{	
		ft_stderr("minishell: %s: file failed to create\n",
			redir_file->file);
		return (false);
	}
	return (true);
}

/**
 * @brief Calls functions to check for valid redir files, 
 * returns the out_fd which if the fd for the file opened/created.
 * 
 * @param step 
 * @return int 
 */
int	exec_outredir(t_exec_step *step)
{
	t_list	*redir;
	t_redir	*redir_file;
	int		out_fd;
	int		check;

	out_fd = -1;
	if (step->cmd == NULL)
		return (out_fd);
	redir = step->cmd->redirs;
	while (redir)
	{
		redir_file = redir->content;
		check = check_access_for_redir(redir_file, &redir);
		if (check == 1)
			break ;
		else if (check == 2)
			continue ;
		if (check_redir_file_exist(redir_file, &out_fd) == false)
			return (out_fd);
		else
			if (create_redir_file(redir_file, &out_fd) == false)
				return (-2);
		redir = redir->next;
	}
	return (out_fd);
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

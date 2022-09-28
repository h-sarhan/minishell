/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 21:07:52 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/28 14:44:03 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Checks if the file associated with input 
 * redirection is valid or not.
 * 
 * @param redir_file 
 * @return true 
 * @return false 
 */
bool	check_input_redir(t_redir	*redir_file)
{
	if (redir_file->type == INPUT_REDIR)
	{
		if (access(redir_file->file, F_OK) == -1)
		{
			ft_stderr("minishell: %s: No such file or directory\n",
				redir_file->file);
			return (false);
		}
		if (access(redir_file->file, R_OK) == -1)
		{
			ft_stderr("minishell: %s: Permission denied\n",
				redir_file->file);
			return (false);
		}
	}
	return (true);
}

/**
 * @brief Checks if the file associated with output 
 * redirection is valid or not.
 * 
 * @param redir_file 
 * @return true 
 * @return false 
 */
bool	check_output_redir(t_redir	*redir_file)
{
	if (redir_file->type == OUTPUT_REDIR || redir_file->type == APPEND)
	{
		if ((access(redir_file->file, F_OK) != -1
				&& access(redir_file->file, W_OK) == -1)
			|| is_dir(redir_file->file))
		{
			ft_stderr("minishell: %s: Permission denied\n",
				redir_file->file);
			return (false);
		}
	}
	return (true);
}

/**
 * @brief Checks if the file given in the redirection
 * is valid or not.
 * 
 * @param step 
 * @return true 
 * @return false 
 */
bool	check_valid_redir(t_exec_step *step)
{
	t_list	*redir;
	t_redir	*redir_file;

	if (step->cmd == NULL)
		return (true);
	redir = step->cmd->redirs;
	while (redir)
	{
		redir_file = redir->content;
		if (check_input_redir(redir_file) == false)
			return (false);
		else if (check_output_redir(redir_file) == false)
			return (false);
		redir = redir->next;
	}
	return (true);
}

/**
 * @brief Checks if the file given in the redirection argument exists.
 * 
 * @param redir_file 
 * @param out_fd 
 * @return true 
 * @return false 
 */
bool	check_redir_file_exist(t_redir *redir_file, int *out_fd)
{
	if (access(redir_file->file, W_OK) == 0)
	{
		ft_close(out_fd);
		if (redir_file->type == APPEND)
			*out_fd = open(redir_file->file, O_WRONLY | O_APPEND);
		else
			*out_fd = open(redir_file->file, O_WRONLY | O_TRUNC);
		if (*out_fd == -1)
			ft_stderr("minishell: %s: failed to open\n", redir_file->file);
		return (true);
	}
	else if ((access(redir_file->file, F_OK) != -1
			&& access(redir_file->file, W_OK) == -1)
		|| is_dir(redir_file->file))
		return (false);
	return (true);
}

/**
 * @brief Checks if the files specified in the redirection argument exists,
 * and are accessible.
 * 
 * @param redir_file 
 * @param redir 
 * @return int 
 */
int	check_access_for_redir(t_redir *redir_file, t_list **redir)
{
	if (redir_file->type == INPUT_REDIR)
	{
		if (access(redir_file->file, R_OK) == -1)
			return (1);
	}
	else if (redir_file->type == OUTPUT_REDIR || redir_file->type == APPEND)
	{
		if ((access(redir_file->file, F_OK) != -1
				&& access(redir_file->file, W_OK) == -1)
			|| is_dir(redir_file->file))
			return (1);
	}
	if (redir_file->type == INPUT_REDIR || redir_file->type == HEREDOC)
	{
		*redir = (*redir)->next;
		return (2);
	}
	return (0);
}

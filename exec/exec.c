/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:16:54 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/27 08:49:43 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_dupstdin;

/**
 * @brief Checks if the dir/path to the directory exists.
 * 
 * @param path 
 * @return int 
 */
int	is_dir(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

/**
 * @brief A wrapper functions which calls the clos function.
 * It closes the fds if its != -1 (Invalid Fds).
 * 
 * @param fd 
 */
void	ft_close(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

/**
 * @brief Joins the path to the command.
 * 
 * @param bin 
 * @param paths 
 * @param path 
 */
void	join_path(char *bin, char **paths, char **path)
{
	int	i;

	i = 0;
	while (bin != NULL && paths != NULL && paths[i] != NULL)
	{
		*path = ft_strjoin(paths[i], bin);
		if (*path == NULL || access(*path, X_OK) != -1)
			break ;
		ft_free(path);
		i++;
	}
}

/**
 * @brief Get the full path of a command.
 * 
 * @param bin 
 * @param env 
 * @return char* 
 */
char	*get_full_path(char *bin, char **env)
{
	int		i;
	char	*path;
	char	**paths;
	char	*bin_cpy;

	i = 0;
	while (env[i] != NULL && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i] == NULL)
		return (NULL);
	paths = ft_split(ft_strchr(env[i], '=') + 1, ':');
	i = 0;
	bin_cpy = ft_strdup(bin);
	bin = strjoin_free("/", bin, 2);
	join_path(bin, paths, &path);
	if (bin == NULL || paths == NULL || paths[i] == NULL)
		path = NULL;
	ft_free(&bin);
	free_split_array(paths);
	if (path == NULL)
		return (bin_cpy);
	ft_free(&bin_cpy);
	return (path);
}

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
	int 	check;

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

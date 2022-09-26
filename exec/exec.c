/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:16:54 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/26 18:37:28 by mkhan            ###   ########.fr       */
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

int	exec_outredir(t_exec_step *step)
{
	t_list	*redir;
	t_redir	*redir_file;
	int		out_fd;

	out_fd = -1;
	redir = step->cmd->redirs;
	while (redir)
	{
		redir_file = redir->content;
		if (redir_file->type == INPUT_REDIR)
		{
			if (access(redir_file->file, R_OK) == -1)
				break ;
		}
		else if (redir_file->type == OUTPUT_REDIR || redir_file->type == APPEND)
		{
			if ((access(redir_file->file, F_OK) != -1
					&& access(redir_file->file, W_OK) == -1)
				|| is_dir(redir_file->file))
				break ;
		}
		if (redir_file->type == INPUT_REDIR || redir_file->type == HEREDOC)
		{
			redir = redir->next;
			continue ;
		}
		// if ( == false)
		// 	return (out_fd);
		
		if (access(redir_file->file, W_OK) == 0)
		{
			ft_close(&out_fd);
			if (redir_file->type == APPEND)
				out_fd = open(redir_file->file, O_WRONLY | O_APPEND);
			else
				out_fd = open(redir_file->file, O_WRONLY | O_TRUNC);
			if (out_fd == -1)
				ft_stderr("minishell: %s: failed to open\n", redir_file->file);
		}
		else if ((access(redir_file->file, F_OK) != -1
				&& access(redir_file->file, W_OK) == -1)
			|| is_dir(redir_file->file))
			return (out_fd);
		else
		{
			ft_close(&out_fd);
			if (redir_file->type == APPEND)
				out_fd = open(redir_file->file,
						O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				out_fd = open(redir_file->file,
						O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (out_fd == -1)
			{	
				ft_stderr("minishell: %s: file failed to create\n",
					redir_file->file);
				return (-2);
			}
		}
		redir = redir->next;
	}
	return (out_fd);
}


int	*first_cmd(t_exec_step *step, int *fd, t_shell *shell, int out_fd)
{
	int			in_fd;
	int			hd_fd[2];
	int			exitcode;
	int			exit_code;
	t_redir		*inredir;

	inredir = NULL;
	in_fd = -1;
	hd_fd[0] = -1;
	hd_fd[1] = -1;
	if (parent_builtin(step) && !step->pipe_next)
	{
		// ! We dont dup2 input/output/heredoc redirections here
		run_builtin(step, shell, false);
		if (ft_strcmp(step->cmd->arg_arr[0], "exit") == 0)
		{
			exitcode = step->exit_code;
			if (step->cmd->arg_arr)
			{
				ft_lstclear(&shell->tokens, free_token);
				ft_lstclear(&shell->steps, free_exec_step);
				free_split_array(shell->env);
				ft_free(&fd);
			}
			exit(exitcode);
		}
		return (fd);
	}
	if (step->pipe_next)
		pipe(fd);
	if (step->cmd->redirs)
	{
		inredir = last_inredir(step->cmd->redirs);
		if (inredir != NULL)
		{
			if (inredir->type == INPUT_REDIR)
				in_fd = open(inredir->file, O_RDONLY);
			else
			{
				pipe(hd_fd);
				ft_putstr_fd(step->cmd->heredoc_contents, hd_fd[1]);
			}
		}
	}
	if (step->cmd->arg_arr[0] != NULL)
		step->cmd->pid = fork();
	if (step->cmd->arg_arr[0] != NULL && step->cmd->pid == 0)
	{
		if (inredir && inredir->type == HEREDOC)
		{
			ft_close(&hd_fd[1]);
			dup2(hd_fd[0], 0);
		}
		if (step->pipe_next)
		{
			ft_close(&fd[0]);
			dup2(fd[1], 1);
		}
		if (in_fd != -1)
			dup2(in_fd, 0);
		if (out_fd != -1)
			dup2(out_fd, 1);
		if (is_builtin(step))
		{
			run_builtin(step, shell, true);
			ft_close(&fd[1]);
			ft_close(&fd[0]);
			ft_close(&hd_fd[0]);
			ft_close(&hd_fd[1]);
			ft_close(&g_dupstdin);
			exit_code = step->exit_code;
			ft_lstclear(&shell->tokens, free_token);
			free_steps(&shell->steps_to_free);
			ft_close(&out_fd);
			ft_close(&in_fd);
			free_split_array(shell->env);
			free_split_array(shell->declared_env);
			ft_free(&fd);
			exit(exit_code);
		}
		execve(step->cmd->arg_arr[0], step->cmd->arg_arr, shell->env);
	}
	ft_close(&in_fd);
	ft_close(&out_fd);
	ft_close(&hd_fd[0]);
	ft_close(&hd_fd[1]);
	if (step->pipe_next)
		ft_close(&fd[1]);
	return (fd);
}

int	*mid_cmd(t_exec_step *step, int *fd, t_shell *shell, int out_fd)
{
	int			in_fd;
	int			fdtmp;
	int			hd_fd[2];
	int			exit_code;
	t_redir		*inredir;

	in_fd = -1;
	fdtmp = fd[0];
	inredir = NULL;
	hd_fd[0] = -1;
	hd_fd[1] = -1;
	if (step->pipe_next)
		pipe(fd);
	if (step->cmd->redirs)
	{
		inredir = last_inredir(step->cmd->redirs);
		if (inredir != NULL)
		{
			if (inredir->type == INPUT_REDIR)
				in_fd = open(inredir->file, O_RDONLY);
			else
			{
				pipe(hd_fd);
				ft_putstr_fd(step->cmd->heredoc_contents, hd_fd[1]);
			}
		}
	}
	if (step->cmd->arg_arr[0] != NULL)
		step->cmd->pid = fork();
	if (step->cmd->arg_arr[0] != NULL && step->cmd->pid == 0)
	{
		dup2(fdtmp, 0);
		if (inredir && inredir->type == HEREDOC)
		{
			ft_close(&hd_fd[1]);
			ft_close(&fdtmp);
			dup2(hd_fd[0], 0);
		}
		if (step->pipe_next)
		{	
			ft_close(&fd[0]);
			dup2(fd[1], 1);
		}
		if (in_fd != -1)
		{
			dup2(in_fd, 0);
		}
		if (out_fd != -1)
		{
			dup2(out_fd, 1);
		}
		if (is_builtin(step))
		{
			run_builtin(step, shell, true);
			ft_close(&fd[1]);
			ft_close(&fd[0]);
			ft_close(&hd_fd[0]);
			ft_close(&hd_fd[1]);
			ft_close(&g_dupstdin);
			ft_close(&fdtmp);
			exit_code = step->exit_code;
			ft_lstclear(&shell->tokens, free_token);
			free_steps(&shell->steps_to_free);
			free_split_array(shell->env);
			free_split_array(shell->declared_env);
			ft_close(&out_fd);
			ft_close(&in_fd);
			ft_free(&fd);
			exit(exit_code);
		}
		execve(step->cmd->arg_arr[0], step->cmd->arg_arr, shell->env);
	}
	if (!step->pipe_next)
		ft_close(&fd[0]);
	ft_close(&in_fd);
	ft_close(&out_fd);
	ft_close(&hd_fd[0]);
	ft_close(&hd_fd[1]);
	ft_close(&fd[1]);
	ft_close(&fdtmp);
	return (fd);
}

void	exec_cmd(t_shell *shell, t_list *exec_steps,
	int step_number, char *current_line)
{
	t_exec_step	*step;
	t_list		*steps;
	t_list		*sub_tokens;
	t_list		*sub_steps;
	t_list		*tokens;
	t_list		*new_steps;
	bool		flag;
	bool		exit_flag;
	bool		success;
	bool		valid_redirs;
	int			out_fd;
	int			w_status;
	int			*fd;
	int			i;
	int			wait_idx;
	char		*cmd_copy;

	fd = shell->fd;
	fd[0] = -1;
	fd[1] = -1;
	out_fd = -1;
	w_status = 0;
	i = 0;
	steps = exec_steps;
	while (i < step_number && steps != NULL)
	{
		steps = steps->next;
		i++;
	}
	if (steps == NULL)
		return ;
	step = steps->content;
	flag = false;
	exit_flag = false;
	while (steps)
	{
		step_number++;
		step = steps->content;
		if (step->subexpr_line != NULL)
		{
			sub_tokens = tokenize_line(shell, step->subexpr_line, &success);
			sub_steps = parse_tokens(sub_tokens, &success);
			ft_lstclear(&sub_tokens, free_token);
			ft_lstadd_back(&shell->steps_to_free, ft_lstnew(sub_steps));
			exec_cmd(shell, sub_steps, 0, step->subexpr_line);
			if (!flag)
				flag = true;
			if (step->and_next || step->or_next)
				break ;
			steps = steps->next;
			continue ;
		}
		exit_flag = false;
		valid_redirs = check_valid_redir(step);
		if (valid_redirs == false)
		{
			exit_flag = true;
			step->exit_code = 1;
			shell->last_exit_code = step->exit_code;
		}
		out_fd = exec_outredir(step);
		if (out_fd == -2)
		{
			valid_redirs = false;
			exit_flag = true;
			step->exit_code = 1;
			out_fd = -1;
			shell->last_exit_code = step->exit_code;
		}
		if (step->cmd->arg_arr[0] && step->cmd->arg_arr[0][0] != '\0'
			&& (access(step->cmd->arg_arr[0], X_OK) == -1
			&& !is_builtin(step) && !is_dir(step->cmd->arg_arr[0])))
		{
			cmd_copy = get_full_path(step->cmd->arg_arr[0], shell->env);
			if (cmd_copy != NULL)
				step->cmd->arg_arr[0] = cmd_copy;
		}
		if (step->cmd->arg_arr[0] != NULL
			&& (access(step->cmd->arg_arr[0], X_OK) != -1
				&& !ft_strchr(step->cmd->arg_arr[0], '/')))
		{
			ft_stderr("minishell: %s: command not found\n",
				step->cmd->arg_arr[0]);
			exit_flag = true;
			step->exit_code = 127;
			shell->last_exit_code = step->exit_code;
			ft_close(&fd[0]);
			fd[0] = open("/dev/null", O_RDONLY);
			if (!flag)
				flag = true;
			if (step->and_next || step->or_next)
				break ;
			steps = steps->next;
			continue ;
		}
		if (step->cmd->arg_arr[0] && ((access(step->cmd->arg_arr[0], X_OK) == -1
					&& !is_builtin(step)) || is_dir(step->cmd->arg_arr[0])
				|| !valid_redirs))
		{
			if (((access(step->cmd->arg_arr[0], F_OK) == -1
						&& !is_builtin(step)) || is_dir(step->cmd->arg_arr[0]))
				&& valid_redirs && !ft_strchr(step->cmd->arg_arr[0], '/'))
			{
				ft_stderr("minishell: %s: command not found\n",
					step->cmd->arg_arr[0]);
				exit_flag = true;
				step->exit_code = 127;
				shell->last_exit_code = step->exit_code;
			}
			else if (access(step->cmd->arg_arr[0], F_OK) != -1
				&& access(step->cmd->arg_arr[0], X_OK) == -1
				&& !ft_strchr(step->cmd->arg_arr[0], '/') && valid_redirs)
			{
				ft_stderr("minishell: %s: command not found\n",
					step->cmd->arg_arr[0]);
				exit_flag = true;
				step->exit_code = 127;
				shell->last_exit_code = step->exit_code;
			}
			else if (is_dir(step->cmd->arg_arr[0]) && valid_redirs)
			{	
				ft_stderr("minishell: %s: is a directory\n",
					step->cmd->arg_arr[0]);
				exit_flag = true;
				step->exit_code = 126;
				shell->last_exit_code = step->exit_code;
			}
			else if ((access(step->cmd->arg_arr[0], F_OK) == -1
					&& !is_builtin(step)) && valid_redirs
				&& ft_strchr(step->cmd->arg_arr[0], '/'))
			{
				ft_stderr("minishell: %s: No such file or directory\n",
					step->cmd->arg_arr[0]);
				exit_flag = true;
				step->exit_code = 127;
				shell->last_exit_code = step->exit_code;
			}
			else if ((access(step->cmd->arg_arr[0], X_OK) == -1
					&& !is_builtin(step)) && valid_redirs)
			{
				ft_stderr("minishell: %s: Permission denied\n",
					step->cmd->arg_arr[0]);
				exit_flag = true;
				step->exit_code = 126;
				shell->last_exit_code = step->exit_code;
			}
			ft_close(&fd[0]);
			fd[0] = open("/dev/null", O_RDONLY);
			if (!flag)
				flag = true;
			if (step->and_next || step->or_next)
				break ;
			steps = steps->next;
			continue ;
		}
		if (!flag && valid_redirs)
		{
			fd = first_cmd(step, fd, shell, out_fd);
			flag = true;
		}
		else if (valid_redirs)
			fd = mid_cmd(step, fd, shell, out_fd);
		if (step->and_next || step->or_next)
			break ;
		steps = steps->next;
	}
	ft_close(&fd[0]);
	ft_close(&out_fd);
	// ! Do not wait for subexpressions
	if (step->cmd)
	{
		steps = exec_steps;
		wait_idx = 0;
		while (steps && wait_idx < i)
		{
			steps = steps->next;
			wait_idx++;
		}
		while (steps && i < step_number)
		{
			step = steps->content;
			if (step->cmd->arg_arr[0] && !is_dir(step->cmd->arg_arr[0])
				&& (access(step->cmd->arg_arr[0], X_OK) != -1
					|| is_builtin(step)))
			{
				waitpid(step->cmd->pid, &w_status, 0);
			}
			if (step->and_next || step->or_next)
				break ;
			i++;
			steps = steps->next;
		}
		if (WIFSIGNALED(w_status))
		{
			if (WTERMSIG(w_status) == SIGINT)
			{
				step->exit_code = 130;
				shell->last_exit_code = step->exit_code;
			}
			if (WTERMSIG(w_status) == SIGQUIT)
			{
				printf("Quit\n");
				step->exit_code = 131;
				shell->last_exit_code = step->exit_code;
			}
			return ;
		}
		if (!exit_flag)
		{
			// Checking the only case where we dont fork
			if (!(((t_exec_step *)exec_steps->content)->pipe_next == false
					&& parent_builtin(exec_steps->content)))
			{
				step->exit_code = WEXITSTATUS(w_status);
				shell->last_exit_code = step->exit_code;
			}
		}
	}
	if (step == NULL)
		return ;
	if (step->and_next)
	{
		if (shell->last_exit_code != 0)
		{
			while (steps != NULL && step->and_next)
			{
				step = steps->content;
				steps = steps->next;
				step_number++;
			}
			if (step->or_next)
				step_number--;
			while (steps && (!step->and_next && !step->or_next))
			{
				step = steps->content;
				steps = steps->next;
				step_number++;
			}
			if (steps == NULL)
				return ;
		}
		ft_lstclear(&shell->tokens, free_token);
		tokens = tokenize_line(shell, current_line, &success);
		new_steps = parse_tokens(tokens, &success);
		ft_lstadd_back(&shell->steps_to_free, ft_lstnew(new_steps));
		shell->tokens = tokens;
		shell->steps = new_steps;
		exec_cmd(shell, new_steps, step_number, current_line);
	}
	else if (step->or_next)
	{
		if (shell->last_exit_code == 0)
		{
			while (steps != NULL && step->or_next)
			{
				step = steps->content;
				steps = steps->next;
				step_number++;
			}
			if (step->and_next)
			{
				step_number--;
			}
			while (steps && (!step->and_next && !step->or_next))
			{
				step = steps->content;
				steps = steps->next;
				step_number++;
			}
			if (steps == NULL)
				return ;
		}
		ft_lstclear(&shell->tokens, free_token);
		tokens = tokenize_line(shell, current_line, &success);
		new_steps = parse_tokens(tokens, &success);
		ft_lstadd_back(&shell->steps_to_free, ft_lstnew(new_steps));
		shell->tokens = tokens;
		shell->steps = new_steps;
		exec_cmd(shell, new_steps, step_number, current_line);
	}
}

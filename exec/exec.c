/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:16:54 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/12 16:28:01 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_dir(const char *path) 
{
   struct stat statbuf;
   
   if (stat(path, &statbuf) != 0)
       return (0);
   return S_ISDIR(statbuf.st_mode);
}

void	ft_close(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
	// else
	// {
	// 	printf
	// 	);
	// }
}

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
	while (bin != NULL && paths != NULL && paths[i] != NULL)
	{
		path = ft_strjoin(paths[i], bin);
		if (path == NULL || access(path, X_OK) != -1)
			break ;
		ft_free(&path);
		i++;
	}
	if (bin == NULL || paths == NULL || paths[i] == NULL)
		path = NULL;
	ft_free(&bin);
	free_split_array(paths);
	if (path == NULL)
		return (bin_cpy);
	ft_free(&bin_cpy);
	return (path);
}

bool	check_valid_redir(t_exec_step *step)
{
	t_list	*redir;
	t_redir	*redir_file;
	
	redir = step->cmd->redirs;
	while (redir)
	{
		redir_file = redir->content;
		if (redir_file->type == INPUT_REDIR)
		{
			if (access(redir_file->file, F_OK) == -1)
			{
				ft_stderr("minishell: %s: No such file or directory\n", redir_file->file);
				return (false);
			}
			if (access(redir_file->file, R_OK) == -1)
			{
				ft_stderr("minishell: %s: Permission denied\n", redir_file->file);
				return (false);
			}
		}
		else if (redir_file->type == OUTPUT_REDIR || redir_file->type == APPEND)
		{
			if ((access(redir_file->file, F_OK) != -1 && access(redir_file->file, W_OK) == -1) || is_dir(redir_file->file))
			{
				ft_stderr("minishell: %s: Permission denied\n", redir_file->file);
				return (false);
			}
		}
		redir = redir->next;
	}
	return (true);
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
				break;
		}
		else if (redir_file->type == OUTPUT_REDIR || redir_file->type == APPEND)
		{
			if ((access(redir_file->file, F_OK) != -1 && access(redir_file->file, W_OK) == -1) || is_dir(redir_file->file))
				break;
		}
		if (redir_file->type == INPUT_REDIR || redir_file->type == HEREDOC)
		{
			redir = redir->next;
			continue;
		}
		if (access(redir_file->file, W_OK) == 0)
		{
			ft_close(&out_fd);
			if (redir_file->type == APPEND)
				out_fd = open(redir_file->file, O_WRONLY | O_APPEND);
			else
				out_fd = open(redir_file->file, O_WRONLY | O_TRUNC);
			if (out_fd == -1)
				ft_stderr("minishell: %s: failed to open\n", redir_file->file);
			// return (out_fd);
		}
		else if ((access(redir_file->file, F_OK) != -1 && access(redir_file->file, W_OK) == -1) || is_dir(redir_file->file))
		{
			return (out_fd);
		}
		else
		{
			ft_close(&out_fd);
			if (redir_file->type == APPEND)
				out_fd = open(redir_file->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				out_fd = open(redir_file->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			// printf("%d", out_fd);
			if (out_fd == -1)
				ft_stderr("minishell: %s: file failed to create\n", redir_file->file);
			// ft_close(&out_fd);
			// return (out_fd);
		}
		redir = redir->next;
	}
	return (out_fd);
}

  t_redir *last_inredir(t_list *in_redir)
  {
	t_redir *last;
	t_redir *current_redir;

	last = NULL;
	while (in_redir)
	{
		current_redir = in_redir->content;
		if (current_redir->type == INPUT_REDIR || current_redir->type == HEREDOC)
			last = current_redir;
		in_redir = in_redir->next;
	}
	return(last);
  }

int	*first_cmd(t_exec_step *step, int *fd, t_shell *shell, int out_fd)
{
	int			in_fd;
	// int			
	int			exitcode;
	t_redir		*inredir;
	
	inredir = NULL;
	in_fd = -1;
	if (fork_builtin(step) && !step->pipe_next)
	{
		run_builtin(step, shell, false);
		if (ft_strcmp(step->cmd->arg_arr[0], "exit") == 0)
		{
			// Hardcoding :(
			exitcode = step->exit_code;
			if (step->cmd->arg_arr)
			{
				ft_lstclear(&shell->tokens, free_token);
				ft_lstclear(&shell->steps, free_exec_step);
				free_split_array(shell->env);
				ft_free(&fd);
				// ft_free(&line);
			}
			exit(exitcode);
		}
		return fd;
	}
	if (step->pipe_next)
		pipe(fd);
	check_valid_redir(step);
	if (step->cmd->redirs)
	{
		inredir = last_inredir(step->cmd->redirs);
		if (inredir != NULL)
		{
			if (inredir->type == INPUT_REDIR)
				in_fd = open(inredir->file, O_RDONLY);
			else
			{
				pipe(fd);
				ft_putstr_fd(step->cmd->heredoc_contents, fd[1]);
			}
			// if (in_fd == -1)
			// 	ft_stderr("minishell: %s: No such file or directory\n", inredir->file);
		}
	}
	if (step->cmd->arg_arr[0] != NULL)
		step->cmd->pid = fork();
	if (step->cmd->arg_arr[0] != NULL && step->cmd->pid == 0)
	{
		if (inredir && inredir->type == HEREDOC)
		{
			// if (!step->pipe_next)
			ft_close(&fd[1]);
			dup2(fd[0], 0);
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
			ft_stderr("GOING IN BUILTIN\n");
			run_builtin(step, shell, true);
			ft_close(&fd[1]);
			ft_close(&fd[0]);
			close(1);
			close(0);
			int	exit_code = step->exit_code;
			ft_lstclear(&shell->tokens, free_token);
			ft_lstclear(&shell->steps, free_exec_step);
			ft_close(&out_fd);
			ft_close(&in_fd);
			free_split_array(shell->env);
			ft_free(&fd);
			exit(exit_code);
		}
		execve(step->cmd->arg_arr[0], step->cmd->arg_arr, shell->env);
		printf("FAIL AT Start\n");
	}
	ft_close(&in_fd);
	ft_close(&out_fd);
	ft_close(&fd[0]);
	ft_close(&fd[1]);
	if (step->pipe_next)
		ft_close(&fd[1]);
	return fd;
}

int	*mid_cmd(t_exec_step *step, int *fd, t_shell *shell, int out_fd)
{
	int	in_fd;
	int	fdtmp;
	t_redir		*inredir;
	
	in_fd = -1;
	fdtmp = fd[0];
	inredir = NULL;
	// if (fork_builtin(step) && !step->pipe_next)
	// {
	// 	run_builtin(step, shell);
	// 	return fd;
	// }
	if (step->pipe_next)
		pipe(fd);
	check_valid_redir(step);
	if (step->cmd->redirs)
	{
		inredir = last_inredir(step->cmd->redirs);
		if (inredir != NULL)
		{
			if (inredir->type == INPUT_REDIR)
				in_fd = open(inredir->file, O_RDONLY);
			// if (in_fd == -1)
			// 	ft_stderr("minishell: %s: No such file or directory\n", inredir->file);
			else
			{
				pipe(fd);
				ft_putstr_fd(step->cmd->heredoc_contents, fd[1]);
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
			// if (!step->pipe_next)
			ft_close(&fd[1]);
			ft_close(&fdtmp);
			dup2(fd[0], 0);
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
			ft_stderr("GOING IN BUILTIN\n");
			// close(2);
			run_builtin(step, shell, true);
			ft_close(&fd[1]);
			ft_close(&fd[0]);
			ft_close(&fdtmp);
			close(1);
			close(0);
			int	exit_code = step->exit_code;
			ft_lstclear(&shell->tokens, free_token);
			ft_lstclear(&shell->steps, free_exec_step);
			free_split_array(shell->env);
			ft_close(&out_fd);
			ft_close(&in_fd);
			ft_free(&fd);
			exit(exit_code);
		}
		execve(step->cmd->arg_arr[0], step->cmd->arg_arr, shell->env);
		printf("FAIL\n");
	}
	if (!step->pipe_next)
		ft_close(&fd[0]);
	ft_close(&in_fd);
	ft_close(&out_fd);
	ft_close(&fd[1]);
	ft_close(&fdtmp);
	return fd;
}


void	exec_cmd(t_shell *shell)
{
	t_exec_step *step;
	t_list		*steps;
	int			*fd;
	bool		flag;
	int			out_fd;

	fd = ft_calloc(2, sizeof(int));
	fd[0] = -1;
	fd[1] = -1;
	out_fd = -1;
	steps = shell->steps;
	flag = false;
	while (steps)
	{
		step = steps->content;
		
		// check_valid_redir(step);
		run_here_docs(step);
		out_fd = exec_outredir(step);
		if (step->cmd->arg_arr[0] &&  (access(step->cmd->arg_arr[0], X_OK) == -1 && !is_builtin(step) && !is_dir(step->cmd->arg_arr[0])))
			step->cmd->arg_arr[0] = get_full_path(step->cmd->arg_arr[0], shell->env);
		
		if (step->cmd->arg_arr[0] && ((access(step->cmd->arg_arr[0], X_OK) == -1 && !is_builtin(step)) || is_dir(step->cmd->arg_arr[0]) || !check_valid_redir(step)))
		{
			if (is_dir(step->cmd->arg_arr[0]))
				ft_stderr("minishell: %s: is a directory\n", step->cmd->arg_arr[0]);
			else if ((access(step->cmd->arg_arr[0], X_OK) == -1 && !is_builtin(step)))
				ft_stderr("minishell: %s: command not found\n", step->cmd->arg_arr[0]);
			steps = steps->next;
			ft_close(&fd[0]);
			fd[0] = open("/dev/null", O_RDWR);
			if (!flag)
				flag = true;
			continue;
		}
		if (!flag)
		{
			fd = first_cmd(step, fd, shell, out_fd);
			flag = true;
		}
		else
			fd = mid_cmd(step, fd, shell, out_fd);
		steps = steps->next;
	}
	ft_close(&fd[0]);
	ft_close(&out_fd);
	ft_free(&fd);
	steps = shell->steps;
	while (steps)
	{
		step = steps->content;
		if (step->cmd->arg_arr[0] && (access(step->cmd->arg_arr[0], X_OK) != -1 || is_builtin(step)) && !is_dir(step->cmd->arg_arr[0]))
		{
			// printf("WAITING FOR %s\n", step->cmd->arg_arr[0]);
			waitpid(step->cmd->pid, 0, 0);
		}
		steps = steps->next;
	}
}

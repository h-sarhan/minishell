/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:16:54 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/06 19:39:09 by mkhan            ###   ########.fr       */
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

bool	check_valid_inredir(t_exec_step *step)
{
	t_list	*redir;
	t_redir	*redir_file;
	
	redir = step->cmd->in_redirs;
	while (redir)
	{
		redir_file = redir->content;
		if (access(redir_file->file, R_OK) == -1 || is_dir(redir_file->file))
		{
			ft_stderr("minishell: %s: Permission denied\n", redir_file->file);
			return (false);
		}
		redir = redir->next;
	}
	return (true);
}

bool	check_valid_outredir(t_exec_step *step)
{
	t_list	*redir;
	t_redir	*redir_file;
	
	redir = step->cmd->out_redirs;
	while (redir)
	{
		redir_file = redir->content;
		if ((access(redir_file->file, F_OK) != -1 && access(redir_file->file, W_OK) == -1) || is_dir(redir_file->file))
		{
			ft_stderr("minishell: %s: Permission denied\n", redir_file->file);
			return (false);
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
	redir = step->cmd->out_redirs;
	while (redir)
	{
		redir_file = redir->content;
		if (access(redir_file->file, W_OK) == 0)
		{
			ft_close(&out_fd);
			out_fd = open(redir_file->file, O_WRONLY | O_TRUNC);
			if (out_fd == -1)
				ft_stderr("minishell: %s: failed\n", redir_file->file);
			// return (out_fd);
		}
		else if ((access(redir_file->file, F_OK) != -1 && access(redir_file->file, W_OK) == -1) || is_dir(redir_file->file))
		{
			return (out_fd);
		}
		else
		{
			ft_close(&out_fd);
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

int	*first_cmd(t_exec_step *step, int *fd, t_shell *shell)
{
	int			in_fd;
	int			out_fd;
	int			exitcode;
	t_redir		*inredir;
	
	in_fd = -1;
	out_fd = -1;
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
	check_valid_inredir(step);
	if (step->cmd->in_redirs)
	{
		inredir = ft_lstlast(step->cmd->in_redirs)->content;
		in_fd = open(inredir->file, O_RDONLY);
		if (in_fd == -1)
			ft_stderr("minishell: %s: No such file or directory\n", inredir->file);
	}
	check_valid_outredir(step);
	if (step->cmd->out_redirs)
		out_fd = exec_outredir(step);
	step->cmd->pid = fork();
	if (step->cmd->pid == 0)
	{
		if (step->pipe_next)
		{
			ft_close(&fd[0]);
			dup2(fd[1], 1);
		}
		if (step->cmd->in_redirs)
			dup2(in_fd, 0);
		if (step->cmd->out_redirs)
			dup2(out_fd, 1);
		if (is_builtin(step))
		{
			ft_stderr("GOING IN BUILTIN\n");
			// if (step->pipe_next)
			// 	close(2);
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
	if (step->pipe_next)
		ft_close(&fd[1]);
	return fd;
}

int	*mid_cmd(t_exec_step *step, int *fd, t_shell *shell)
{
	int fdtmp;
	
	fdtmp = fd[0];
	// if (fork_builtin(step) && !step->pipe_next)
	// {
	// 	run_builtin(step, shell);
	// 	return fd;
	// }
	if (step->pipe_next)
		pipe(fd);
	step->cmd->pid = fork();
	if (step->cmd->pid == 0)
	{
		dup2(fdtmp, 0);
		if (step->pipe_next)
		{	
			ft_close(&fd[0]);
			dup2(fd[1], 1);
		}
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
			ft_free(&fd);
			exit(exit_code);
		}
		execve(step->cmd->arg_arr[0], step->cmd->arg_arr, shell->env);
		printf("FAIL\n");
	}
	if (!step->pipe_next)
		ft_close(&fd[0]);
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

	fd = ft_calloc(2, sizeof(int));
	fd[0] = -1;
	fd[1] = -1;
	steps = shell->steps;
	flag = false;
	while (steps)
	{
		step = steps->content;
		if (access(step->cmd->arg_arr[0], X_OK) == -1 && !is_builtin(step) && !is_dir(step->cmd->arg_arr[0]))
			step->cmd->arg_arr[0] = get_full_path(step->cmd->arg_arr[0], shell->env);
		if ((access(step->cmd->arg_arr[0], X_OK) == -1 && !is_builtin(step)) || is_dir(step->cmd->arg_arr[0]) || !check_valid_inredir(step))
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
			fd = first_cmd(step, fd, shell);
			flag = true;
		}
		else
			fd = mid_cmd(step, fd, shell);
		steps = steps->next;
	}
	ft_close(&fd[0]);
	ft_free(&fd);
	steps = shell->steps;
	while (steps)
	{
		step = steps->content;
		if ((access(step->cmd->arg_arr[0], X_OK) != -1 || is_builtin(step)) && !is_dir(step->cmd->arg_arr[0]))
		{
			printf("WAITING FOR %s\n", step->cmd->arg_arr[0]);
			waitpid(step->cmd->pid, 0, 0);
		}
		steps = steps->next;
	}
}

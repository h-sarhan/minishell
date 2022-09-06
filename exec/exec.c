/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:16:54 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/06 13:16:38 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	*first_cmd(t_exec_step *step, int *fd, t_shell *shell)
{
	int	exitcode;
	if (fork_builtin(step) && !step->pipe_next)
	{
		run_builtin(step, shell);
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
	step->cmd->pid = fork();
	if (step->cmd->pid == 0)
	{
		if (step->pipe_next)
		{
			ft_close(&fd[0]);
			dup2(fd[1], 1);
		}
		if (is_builtin(step))
		{
			ft_stderr("GOING IN BUILTIN\n");
			if (step->pipe_next)
				close(2);
			run_builtin(step, shell);
			ft_close(&fd[1]);
			ft_close(&fd[0]);
			// close(1);
			// close(0);
			int	exit_code = step->exit_code;
			ft_lstclear(&shell->tokens, free_token);
			ft_lstclear(&shell->steps, free_exec_step);
			free_split_array(shell->env);
			ft_free(&fd);
			exit(exit_code);
		}
		execve(step->cmd->arg_arr[0], step->cmd->arg_arr, shell->env);
		printf("FAIL AT Start\n");
	}
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
			close(2);
			run_builtin(step, shell);
			ft_close(&fd[1]);
			ft_close(&fd[0]);
			ft_close(&fdtmp);
			
			// close(1);
			// close(0);
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
#include <fcntl.h>

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
		if (access(step->cmd->arg_arr[0], X_OK) == -1 && !is_builtin(step))
			step->cmd->arg_arr[0] = get_full_path(step->cmd->arg_arr[0], shell->env);
		if (access(step->cmd->arg_arr[0], X_OK) == -1 && !is_builtin(step))
		{
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
		if (access(step->cmd->arg_arr[0], X_OK) != -1 || is_builtin(step))
		{
			printf("WAITING FOR %s\n", step->cmd->arg_arr[0]);
			waitpid(step->cmd->pid, 0, 0);
		}
		steps = steps->next;
	}
}

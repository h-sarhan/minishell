/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:16:54 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/05 16:03:35 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_full_path(char *bin, char **env)
{
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	while (env[i] != NULL && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i] == NULL)
		return (NULL);
	paths = ft_split(ft_strchr(env[i], '=') + 1, ':');
	i = 0;
	bin = strjoin_free("/", bin, 0);
	while (bin != NULL && paths != NULL && paths[i] != NULL)
	{
		path = ft_strjoin(paths[i], bin);
		if (path == NULL || access(path, X_OK) != -1)
			break ;
		// ft_free(&path);
		i++;
	}
	if (bin == NULL || paths == NULL || paths[i] == NULL)
		path = NULL;
	// ft_free(&bin);
	// free_split_array(paths);
	return (path);
}

int	*first_cmd(t_exec_step *step, int *fd, t_shell *shell)
{
	if (step->pipe_next)
		pipe(fd);
	step->cmd->pid = fork();
	if (step->cmd->pid == 0)
	{
		if (step->pipe_next)
		{
			close(fd[0]);
			dup2(fd[1], 1);
		}
		execve(step->cmd->arg_arr[0], step->cmd->arg_arr, shell->env);
		printf("FAIL AT Start\n");
	}
	if (step->pipe_next)
		close(fd[1]);
	return fd;
}

int	*mid_cmd(t_exec_step *step, int *fd, t_shell *shell)
{
	int fdtmp;
	
	fdtmp = fd[0];
	if (step->pipe_next)
		pipe(fd);
	step->cmd->pid = fork();
	if (step->cmd->pid == 0)
	{
		dup2(fdtmp, 0);
		if (step->pipe_next)
		{	
			close(fd[0]);
			dup2(fd[1], 1);
		}
		// printf("step %s\n", step->cmd->arg_arr[0]);
		execve(step->cmd->arg_arr[0], step->cmd->arg_arr, shell->env);
		printf("FAIL\n");
	}
	if (step->pipe_next)
		close(fd[1]);
	else
		close(fdtmp);
	return fd;
}

void	exec_cmd(t_shell *shell)
{
	t_exec_step *step;
	t_list		*steps;
	int			*fd;
	bool		flag;

	fd = ft_calloc(2, sizeof(int));
	steps = shell->steps;
	flag = false;
	// printf("%s\n", get_full_path(step->cmd->arg_arr[0], shell->env));
	while (steps)
	{
		step = steps->content;
		step->cmd->arg_arr[0] = get_full_path(step->cmd->arg_arr[0], shell->env);
		// printf("step %s\n", step->cmd->arg_arr[0]);
		if (!flag)
		{
			fd = first_cmd(step, fd, shell);
			flag = true;
		}
		else
			fd = mid_cmd(step, fd, shell);
		waitpid(step->cmd->pid, 0, 0);
		steps = steps->next;
	}
	
}

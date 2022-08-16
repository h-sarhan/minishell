/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:02:59 by hsarhan           #+#    #+#             */
/*   Updated: 2022/07/04 12:03:29 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Closes 2 fds
void	close_fds(int fd1, int fd2, t_list *first)
{
	close_fd(fd1, first);
	close_fd(fd2, first);
}

// Checks if a command is accessible and prints an error message if it is not
int	command_check(char **cmd_args, char *arg_list, int fd)
{
	char	**args;
	char	*cmd_name;
	int		i;

	if (cmd_args[0] == NULL)
	{
		args = ft_split(arg_list, ' ');
		cmd_name = ft_strdup(args[0]);
		if (args == NULL || cmd_name == NULL)
			return (0);
		free_split_array(args);
		if (fd != -1 && access(cmd_name, X_OK) == -1 && errno == EACCES)
			perror("pipex");
		else if (fd != -1)
			print_error_string("command not found", cmd_name);
		ft_free(cmd_name);
		i = 0;
		while (cmd_args[++i] != NULL)
			ft_free(cmd_args[i]);
		if (errno == EACCES)
			return (EACCES);
		return (0);
	}
	return (1);
}

// Wrapper around pipe() that handles errors
void	ft_pipe(int *pipe_fds, t_list *first)
{
	int	pipe_ret;

	pipe_ret = pipe(pipe_fds);
	if (pipe_ret == -1)
	{
		perror("pipex");
		ft_lstclear(&first, free_cmd);
		exit(EXIT_FAILURE);
	}
}

// Wrapper around fork() that handles errors
int	ft_fork(int command_valid, t_list *first)
{
	int	pid;

	if (command_valid)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("pipex");
			ft_lstclear(&first, free_cmd);
			exit(EXIT_FAILURE);
		}
		return (pid);
	}
	return (-1);
}

// Helper function that prints error messages
void	print_error_string(char *error_str, char *file_name)
{
	char	*str;

	str = ft_strjoin("pipex: ", file_name);
	str = ft_strjoinfree(str, ": ", 1);
	str = ft_strjoinfree(str, error_str, 1);
	if (str == NULL)
		exit(EXIT_FAILURE);
	ft_putendl_fd(str, 2);
	ft_free(str);
}

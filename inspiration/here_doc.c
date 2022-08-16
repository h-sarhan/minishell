/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 10:14:54 by hsarhan           #+#    #+#             */
/*   Updated: 2022/07/04 13:10:13 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Resizes an array by mallocing an array of old_len * 2
// and copies all elements of the old array into the new array
// Finally, the old array is freed
void	*resize(char **arr, int *old_len, char *limiter)
{
	int		i;
	char	*new_arr;

	i = 0;
	new_arr = ft_calloc(*old_len * 2 + 1, sizeof(char));
	if (new_arr == NULL)
	{
		ft_free(*arr);
		ft_free(limiter);
		exit(EXIT_FAILURE);
	}
	while (i < *old_len)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	free(*arr);
	*old_len *= 2;
	return (new_arr);
}

// Reads from STDIN into a buffer until a limiter is found
// Then the buffer is returned
char	*read_from_stdin(char *limiter)
{
	char	*buff;
	int		buffer_len;
	int		i;
	char	ch;

	buffer_len = 100;
	if (limiter == NULL)
		exit(EXIT_FAILURE);
	buff = ft_calloc(buffer_len + 1, sizeof(char));
	if (buff == NULL)
		return (NULL);
	i = 0;
	while (ft_strnstr(buff, limiter, ft_strlen(buff)) == NULL)
	{
		if (i == buffer_len)
			buff = resize(&buff, &buffer_len, limiter);
		if (read(STDIN, &ch, 1) < 1)
			break ;
		buff[i] = ch;
		i++;
	}
	buff[i] = '\0';
	if (ft_strnstr(buff, limiter, ft_strlen(buff)) != NULL)
		ft_strnstr(buff, limiter, ft_strlen(buff))[1] = '\0';
	return (buff);
}

// Runs the first command followed by a here_doc
t_cmd	*run_heredoc_cmd(char **argv, char *buff, int *cmd_pipes, char **env)
{
	int		heredoc_pipes[2];
	t_cmd	*cmd;

	ft_pipe(heredoc_pipes, NULL);
	write(heredoc_pipes[WRITE], buff, ft_strlen(buff));
	cmd = create_command(argv[3], env);
	if (cmd == NULL)
		return (NULL);
	cmd->valid = command_check(cmd->cmd_args, argv[3], 1);
	cmd->pid = ft_fork(1, NULL);
	if (cmd->pid == 0)
	{
		close_fds(cmd_pipes[READ], heredoc_pipes[WRITE], NULL);
		dup_fd(heredoc_pipes[READ], STDIN, NULL, cmd);
		dup_fd(cmd_pipes[WRITE], STDOUT, NULL, cmd);
		if (cmd->valid)
			execve(cmd->cmd_args[0], cmd->cmd_args, env);
		close_fds(heredoc_pipes[READ], cmd_pipes[WRITE], NULL);
		free_cmd(cmd);
		ft_free(buff);
		exit(EXIT_FAILURE);
	}
	close_fds(heredoc_pipes[WRITE], heredoc_pipes[READ], NULL);
	return (cmd);
}

// Handles here_doc behaviour
t_list	*handle_here_doc(char **argv, int *cmd_pipes, int *fds, char **env)
{
	char	*limiter;
	char	*buff;
	t_cmd	*cmd;
	t_list	*cmds;
	int		argc;

	argc = 0;
	while (argv[argc] != NULL)
		argc++;
	ft_pipe(cmd_pipes, NULL);
	fds[1] = open_file(argv[argc - 1], 3);
	limiter = ft_strjoinfree("\n", argv[2], 0);
	limiter = ft_strjoinfree(limiter, "\n", 1);
	buff = read_from_stdin(limiter);
	if (buff == NULL)
		exit(EXIT_FAILURE);
	ft_free(limiter);
	cmd = run_heredoc_cmd(argv, buff, cmd_pipes, env);
	ft_free(buff);
	cmds = create_command_list(argc, argv, fds, env);
	if (cmd == NULL || cmds == NULL)
		return (NULL);
	free_cmd(cmds->content);
	cmds->content = cmd;
	return (cmds);
}

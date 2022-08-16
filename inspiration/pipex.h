/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:02:32 by hsarhan           #+#    #+#             */
/*   Updated: 2022/07/04 12:03:40 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/errno.h>

# define READ 0
# define WRITE 1
# define STDOUT 1
# define STDIN 0

// Struct that stores the arguments, process id, validity,
// in_fd, out_fd, and wait status of each command
struct s_command {
	char	**cmd_args;
	int		pid;
	int		valid;
	int		in_fd;
	int		out_fd;
	int		w_status;
	char	**env;
};
typedef struct s_command	t_cmd;

// utils.c
char	*ft_strjoinfree(char *s1, char *s2, int f);
void	free_split_array(char **arr);
char	**trim_args(char **args);
char	*get_full_path(char *bin, char **env);
void	check_args(int argc, char **argv);

// utils2.c
void	ft_free(void *mem);
void	dup_fd(int fd_1, int fd_2, t_list *first, t_cmd *cmd);
void	close_fd(int fd, t_list *first);
int		open_file(char *file_path, int outfile);
char	**get_args(char *arg, char **env);

// error_handling.c
void	close_fds(int fd1, int fd2, t_list *first);
int		command_check(char **cmd_args, char *arg_list, int fd);
void	print_error_string(char *error_str, char *file_name);
void	ft_pipe(int *pipe_fds, t_list *first);
int		ft_fork(int cmd_valid, t_list *first);

// split_args.c
char	**split_args(char const *s, char c);

// command.c
t_cmd	*create_command(char *cmd_str, char **env);
void	wait_cmd(void *cmd);
void	free_cmd(void *cmd);

// run_commands.c
void	run_first_cmd(t_cmd *cmd, int *pipes, int *fds, t_list *cmds);
void	run_middle_cmd(t_cmd *cmd, int *pipes, int *fds, t_list *cmds);
void	run_last_cmd(t_cmd *cmd, int *pipes, int *fds, t_list *cmds);

// pipex.c
t_list	*create_command_list(int argc, char **argv, int *fds, char **env);
void	handle_first_cmd(t_cmd *cmd, int *fds, int *pipes, t_list *cmds);
t_list	*handle_mid_cmds(t_list *cmds, int *pfds, int *fds, t_list *first);
void	wait_and_exit(int *pipe_fds, int *fds, t_list *command_list);
void	handle_last_cmd(t_cmd *cmd, int *fds, int *pipes, t_list *cmds);

// here_doc.c
t_list	*handle_here_doc(char **argv, int *cmd_pipes, int *fds,
			char **env);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:17:33 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/27 17:02:47 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../parsing/parsing.h"
struct	s_exec_flags
{
	bool		first_flag;
	bool		exit_flag;
	bool		valid_redirs;
	int			w_status;
};

typedef struct s_exec_flags t_exec_flags;
void	exec_cmds(t_shell *shell, t_list *steps,
			int step_number, char *current_line);
char	*read_from_stdin(t_shell *shell, char *limiter);
void	run_here_docs(t_shell *shell, t_exec_step *step);
int		*first_cmd(t_exec_step *step, int *fd, t_shell *shell, int out_fd);
t_redir	*last_inredir(t_list *in_redir);
int		cmd_init(t_redir **inredir, t_exec_step *step, int *heredoc_fds,
			int *fds);
int		open_last_redir(t_exec_step *step, int *heredoc_fds, t_redir **inredir);
int		*mid_cmd(t_exec_step *step, int *fds, t_shell *shell, int out_fd);
int		run_child_builtin(t_shell *shell, t_exec_step *step, int *fds,
			int *heredoc_fds);
int		*cmd_cleanup(int *fds, int *in_fd, int *out_fd,
			int *heredoc_fds);
int		is_dir(const char *path);
char	*get_full_path(char *bin, char **env);
bool	check_valid_redir(t_exec_step *step);
int		exec_outredir(t_exec_step *step);

#endif
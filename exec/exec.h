/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:17:33 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/29 23:46:40 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../parsing/parsing.h"

# define CONT 1
# define BREAK -1
# define PASS 0

struct	s_exec_flags
{
	bool	first_flag;
	bool	exit;
	bool	valid_redirs;
	int		w_status;
	int		action;
	int		step_num_start;
	int		step_num;
};

typedef struct s_exec_flags	t_exec_flags;

bool		exec_subexpr(t_shell *shell, t_exec_step *step, t_exec_flags *flags,
				t_list **steps);
void		set_cmd_path(t_shell *shell, t_exec_step *step);
t_list		*reset_to_step(t_list *exec_steps, int step_number_start);
void		init_exec_cmds(t_shell *shell, int *out_fd, t_exec_flags *flags,
				int step_number);
t_list		*go_to_step(t_exec_flags *flags, t_list *exec_steps,
				t_exec_step **step);

/* ----- EXECUTE THE COMMANDS ----- */
void		exec_cmds(t_shell *shell, t_list *exec_steps, int step_number,
				char *current_line);

/* ----- FUNCTION CALLS TO RUN THE COMMANDS ----- */
int			*first_cmd(t_exec_step *step, int *fd, t_shell *shell, int out_fd);
int			*mid_cmd(t_exec_step *step, int *fds, t_shell *shell, int out_fd);

/* ----- CMD UTILS -- CLOSE PIPES -- CHILD BUILTINS ----- */
int			cmd_init(t_shell *shell, t_redir **inredir, t_exec_step *step,
				int *heredoc_fds);
int			*cmd_cleanup(int *fds, int *in_fd, int *out_fd,
				int *heredoc_fds);
int			run_child_builtin(t_shell *shell, t_exec_step *step, int *fds,
				int *heredoc_fds);
void		pipe_fds(const t_exec_step *step, int *fds);

/* ----- RUN HEREDOCS ----- */
// void		run_here_docs(t_shell *shell, t_exec_step *step);
t_list		*run_here_docs(t_shell *shell, t_list *steps);

/* ----- IMPLEMENT && || AND REPARSE AGAIN DURING EXECUTION ----- */
void		handle_and_or(t_shell *shell, t_exec_step *step, int step_number,
				t_list **steps);

/* ----- FUNTIONS TO HANDLE WAITING FOR COMMANDS RAN AND EXIT ----- */
int			get_exit(t_list *exec_steps, t_exec_step *step,
				t_exec_flags *flags);
t_list		*wait_and_get_exit(t_shell *shell, t_exec_step *step,
				t_list *exec_steps, t_exec_flags *flags);

/* ----- HANDLE INVALIDATIONS AND CALL CHECK FUNCTIONS ----- */
void		check_command(t_shell *shell, t_list **steps, t_exec_step *step,
				t_exec_flags *flags);

/* ----- FUNCITONS TO PRINT THE ERR MESSAGE FOR INVALID COMMANDS/FILES ----- */
void		cmd_not_found(t_shell *shell, t_exec_step *step, bool *exit_flag);
void		cmd_is_dir(t_shell *shell, t_exec_step *step, bool *exit_flag);
void		file_not_found(t_shell *shell, t_exec_step *step, bool *exit_flag);
void		permission_denied(t_shell *shell, t_exec_step *step,
				bool *exit_flag);

/* ----- FUNCTIONS TO CHECK FOR COMMAND AND FILE VALIDATIONS ----- */
bool		check_invalid_path(t_exec_step *step);
bool		check_invalid_command(t_exec_step *step, bool valid_redirs);
bool		cmd_not_found_check(t_exec_step *step, bool valid_redirs);
bool		file_not_found_check(t_exec_step *step, bool valid_redirs);
bool		permission_denied_check(t_exec_step *step, bool valid_redirs);

/* ----- FUNCTION TO HANDLE REDIRECTIONS -----*/	
t_redir		*last_inredir(t_list *in_redir);
bool		open_redirs(t_shell *shell, t_exec_step *step,
				bool *exit_flag, int *out_fd);

/* ----- SOME UTILS ----*/	
int			is_dir(const char *path);
void		ft_close(int *fd);
char		*get_full_path(char *bin, char **env);

/* ----- FUNCITON TO CHECK VALIDATION OF REDIRECTION -----*/	
int			check_access_for_redir(t_redir *redir_file, t_list **redir);
bool		check_valid_redir(t_exec_step *step);
bool		check_redir_file_exist(t_redir *redir_file, int *out_fd);

#endif	
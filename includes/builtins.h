/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:46:50 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/30 08:34:48 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "parsing.h"

typedef struct s_shell	t_shell;

/* ---- BUILTINS CHECK FUNCTIONS ---- */
bool	run_builtin(t_exec_step *step, t_shell *shell, bool child);
int		ft_strcmp(char *s1, char *s2);
bool	is_builtin(t_exec_step *step);
bool	parent_builtin(t_exec_step *step);

/* ---- ECHO - PWD - ENV - EXIT - FUNCTIONS ---- */
void	ft_echo(t_exec_step *command, t_shell *shell);
void	ft_pwd(t_exec_step *command, t_shell *shell);
void	ft_env(t_shell *shell, t_exec_step *step);
void	ft_exit(t_exec_step *step, t_shell *shell, bool child);

/* ---- CD FUNCTIONS ---- */
void	ft_cd(t_exec_step *step, char **env, t_shell *shell);
void	find_and_update_oldpwd(t_shell *shell, char **env, char *oldpwd);
void	find_and_update_pwd(char **env, t_shell *shell);
void	move_to_home(t_shell *shell, t_exec_step *step, char **env, char *home);

/* ---- EXPORT FUNCTIONS ---- */
void	ft_export(t_shell *shell, t_exec_step *step);
size_t	env_len(char **env);
void	update_env(t_shell *shell, const char *str);
void	update_declared_env(t_shell *shell, char *str);
bool	check_export_arg(const char *arg);

void	export_error(char *arg);
char	**resize_str_arr(char **old, size_t new_size);
bool	assign_val_to_env(t_shell *shell, char *to_look,
			char *key, char *key_val);
void	print_the_export_env(char **val, char **key);
void	run_export_with_args(char **args, t_shell *shell, bool *error);

/* ---- UNSET FUNCTIONS ---- */
void	ft_unset(t_shell *shell, t_exec_step *step);
void	unset_var(t_shell *shell, const char *var);
void	remove_env_var(t_shell *shell, char **env_copy, char *to_look);
void	unset_declared_var(t_shell *shell, const char *var);
void	remove_declared_env_var(t_shell *shell, char **env_copy, char *to_look);

#endif
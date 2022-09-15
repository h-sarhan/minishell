/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:46:50 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/15 14:45:54 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

#include "../parsing/parsing.h"

typedef struct s_shell	t_shell;

bool 	run_builtin(t_exec_step *step, t_shell *shell, bool child);
int		ft_strcmp(char *s1, char *s2);
void	ft_echo(t_exec_step *command, t_shell *shell);
void	ft_pwd(t_exec_step *command, t_shell *shell);
void	ft_cd(t_exec_step *step, char **env, t_shell *shell);
void	ft_env(t_shell *shell, t_exec_step *step);
void	ft_export(t_shell *shell, t_exec_step *step);
void	ft_unset(t_shell *shell, t_exec_step *step);
void	ft_exit(t_exec_step *step, t_shell *shell, bool child);
char	*get_env(const t_shell *shell, const char *name);
size_t	env_len(char **env);
bool	is_builtin(t_exec_step *step);
bool	fork_builtin(t_exec_step *step);
void	find_and_update_oldpwd(t_shell *shell, char **env, char *oldpwd);
void	unset_var(t_shell *shell, const char *var);
void	update_env(t_shell *shell, const char *str);

#endif
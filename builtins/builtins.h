/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:46:50 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/14 20:30:07 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

#include "../parsing/parsing.h"

typedef struct s_shell	t_shell;

bool 	run_builtin(t_exec_step *step, t_shell *shell, bool child);
int		ft_strcmp(char *s1, char *s2);
void	ft_echo(t_exec_step *command);
void	ft_pwd(t_exec_step *command);
void	ft_cd(t_exec_step *step, char **env);
void	ft_env(const t_shell *shell, t_exec_step *step);
void	ft_export(t_shell *shell, t_exec_step *step);
void	ft_unset(t_shell *shell, t_exec_step *step);
void	ft_exit(t_exec_step *step, t_shell *shell, bool child);
char	*get_env(const t_shell *shell, const char *name);
size_t	env_len(char **env);
bool	is_builtin(t_exec_step *step);
bool	fork_builtin(t_exec_step *step);
#endif
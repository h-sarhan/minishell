/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:46:50 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/01 18:26:33 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

#include "../parsing/parsing.h"

typedef struct s_shell	t_shell;

bool 	is_builtin(t_exec_step *step, t_shell *shell, char *line);
int		ft_strcmp(char *s1, char *s2);
void	ft_echo(t_exec_step *command);
void	ft_pwd(t_exec_step *command);
void	ft_cd(t_exec_step *step, char **env);
void	ft_env(const t_shell *shell, t_exec_step *step);
void	ft_export(t_shell *shell, const t_exec_step *step);
void	ft_exit(t_exec_step *step, t_shell *shell, char *line);
char	*get_env(const t_shell *shell, const char *name);

#endif
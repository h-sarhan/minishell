/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_built_ins.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 17:31:18 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/30 21:28:52 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_BUILT_INS_H
# define ENV_BUILT_INS_H

typedef struct s_shell	t_shell;

void	ft_env(const t_shell *shell, t_exec_step *step);
char	*get_env(const t_shell *shell, const char *name);
void	ft_export(t_shell *shell, const char *name, const char *value);
#endif
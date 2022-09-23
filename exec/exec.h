/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:17:33 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/23 07:50:59 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "../parsing/parsing.h"

void	exec_cmd(t_shell *shell, t_list *exec_steps, int step_number);
char	*read_from_stdin(char *limiter);
void	run_here_docs(t_exec_step *step);


#endif
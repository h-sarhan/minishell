/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:17:33 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/21 17:47:34 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "../parsing/parsing.h"

void	exec_cmd(t_shell *shell, t_list *steps, int step_number, char *current_line);
char	*read_from_stdin(char *limiter);
void	run_here_docs(t_exec_step *step);


#endif
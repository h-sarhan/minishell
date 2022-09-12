/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:17:33 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/12 13:42:00 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "../parsing/parsing.h"

void	exec_cmd(t_shell *shell);
char	*read_from_stdin(char *limiter);

#endif
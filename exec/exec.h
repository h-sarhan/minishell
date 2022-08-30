/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:46:50 by mkhan             #+#    #+#             */
/*   Updated: 2022/08/30 13:52:32 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "../parsing/parsing.h"

void	ft_echo(t_exec_step *command);
void	ft_pwd(t_exec_step *command);

#endif
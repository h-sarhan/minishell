/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 19:40:32 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/25 19:52:37 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "../minishell.h"

void	handle_eof(const char *line, t_shell *shell);
void	handle_ctrl_c(t_shell *shell);
bool	handle_heredoc_ctrlc(t_shell *shell, char *line);

#endif
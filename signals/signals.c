/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 19:39:40 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/25 23:17:54 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_eof(const char *line, t_shell *shell)
{
	if (line == NULL)
	{
		printf("exit\n");
		free_split_array(shell->env);
		free_split_array(shell->declared_env);
		ft_close(&g_dupstdin);
		ft_free(&shell->fd);
		clear_history();
		exit(shell->last_exit_code);
	}
}

void	handle_ctrl_c(t_shell *shell)
{
	if (g_dupstdin == -1)
	{
		g_dupstdin = dup(0);
		shell->last_exit_code = 1;
	}
}

bool	handle_heredoc_ctrl_c(t_shell *shell, char *line)
{
	if (g_dupstdin == -1)
	{
		shell->last_exit_code = 1;
		ft_lstclear(&shell->tokens, free_token);
		free_steps(&shell->steps_to_free);
		rl_on_new_line();
		ft_free(&line);
		ft_close(&g_dupstdin);
		return (false);
	}
	return (true);
}

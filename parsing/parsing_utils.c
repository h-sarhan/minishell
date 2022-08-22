/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 21:38:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/22 07:20:27 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_redir(void *redir_ptr)
{
	t_redir	*redir;

	redir = redir_ptr;
	ft_free(&redir->file);
	ft_free(&redir->limiter);
	ft_free(&redir);
}

void	free_exec_step(void *exec_step_ptr)
{
	t_exec_step	*exec_step;

	exec_step = exec_step_ptr;
	if (exec_step->cmd != NULL)
		ft_lstclear(&exec_step->cmd->args, free);
	if (exec_step->cmd != NULL)
		ft_lstclear(&exec_step->cmd->redirs, free_redir);
	if (exec_step->subexpr_steps != NULL)
		ft_lstclear(&exec_step->subexpr_steps, free_exec_step);
	ft_free(&exec_step->cmd);
	ft_free(&exec_step);
}

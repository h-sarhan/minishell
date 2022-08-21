/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 21:38:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 22:13:30 by hsarhan          ###   ########.fr       */
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
	ft_lstclear(&exec_step->cmd->args, free);
	ft_lstclear(&exec_step->cmd->redirs, free_redir);
	ft_free(&exec_step->cmd);
	ft_free(&exec_step);
}
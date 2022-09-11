/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 21:38:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/11 13:46:58 by hsarhan          ###   ########.fr       */
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
	size_t	i;

	exec_step = exec_step_ptr;
	if (exec_step->cmd != NULL)
	{

		ft_lstclear(&exec_step->cmd->redirs, free_redir);
	}
	if (exec_step->subexpr_steps != NULL)
		ft_lstclear(&exec_step->subexpr_steps, free_exec_step);
	i = 0;
	while (exec_step->cmd->arg_arr[i] != NULL)
	{
		ft_free(&exec_step->cmd->arg_arr[i]);
		i++;
	}
	ft_free(&exec_step->cmd->arg_arr);
	// if (exec_step->cmd != NULL)
	// 	ft_lstclear(&exec_step->cmd->args, free);
	ft_free(&exec_step->cmd);
	ft_free(&exec_step);
}

void	list_to_str_arr(void *step_ptr)
{
	t_list		*arg_list;
	size_t		i;
	size_t		list_size;
	t_exec_step	*step;

	step = step_ptr;
	arg_list = step->cmd->args;
	list_size = ft_lstsize(arg_list);
	step->cmd->arg_arr = ft_calloc(list_size + 1, sizeof(char *));
	if (step->cmd->arg_arr == NULL)
	{
		// ???
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < list_size)
	{
		step->cmd->arg_arr[i] = ft_strdup(arg_list->content);
		arg_list = arg_list->next;
		i++;
	}
	ft_lstclear(&step->cmd->args, free);
	step->cmd->arg_arr[i] = NULL;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_subexprs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 19:35:44 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/25 19:36:01 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	check_subexpr(t_shell *shell, t_exec_step *step)
{
	t_list		*tokens;
	bool		success;
	t_list		*steps;

	tokens = tokenize_line(shell, step->subexpr_line, &success);
	if (success == false)
	{
		ft_lstclear(&tokens, free_token);
		return (false);
	}
	steps = parse_tokens(tokens, &success);
	ft_lstclear(&tokens, free_token);
	if (success == false || check_subexprs(shell, steps) == false)
	{
		ft_lstclear(&steps, free_exec_step);
		return (false);
	}
	ft_lstclear(&steps, free_exec_step);
	return (true);
}

bool	check_subexprs(t_shell *shell, t_list *shell_steps)
{
	t_exec_step	*step;

	while (shell_steps != NULL)
	{
		step = shell_steps->content;
		if (step->subexpr_line != NULL)
		{
			if (check_subexpr(shell, step) == false)
			{
				return (false);
			}
		}
		shell_steps = shell_steps->next;
	}
	return (true);
}

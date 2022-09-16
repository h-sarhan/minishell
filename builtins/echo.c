/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:45:38 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/16 17:49:22 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool ft_check(char *s1)
{
	int i;

	i = 0;
	if (!s1)
		return (false);
	if (s1[0] != '-')
		return (false);
	i = 1;
	if (s1[i] == '\0')
		return (false);
	while (s1[i])
	{
		if (s1[i] == 'n')
			i++;
		else
			return (false);
	}
	return (true);
}

void ft_echo(t_exec_step *step, t_shell *shell)
{
	int	i;
	int n;
	int seen;
	
	i = 1;
	n = 0;
	seen = 0;
	if (ft_check(step->cmd->arg_arr[1]) == true)
		n = 1;
	while (step->cmd->arg_arr[i])
	{
		if (seen)
		{
			// printf("%s", step->cmd->arg_arr[i]);
			ft_putstr_fd(step->cmd->arg_arr[i], 1);
				if (step->cmd->arg_arr[i + 1])
					ft_putstr_fd(" ", 1);
					// printf(" ");
		}
		if (ft_check(step->cmd->arg_arr[i]) != true && !seen)
		{
			seen = 1;
			ft_putstr_fd(step->cmd->arg_arr[i], 1);
			// printf("%s", step->cmd->arg_arr[i]);
			if (step->cmd->arg_arr[i + 1])
				ft_putstr_fd(" ", 1);
				// printf(" ");
		}
		i++;
	}
	if (n == 0)
		// printf("\n");
		ft_putstr_fd("\n", 1);
	step->exit_code = 0;
	shell->last_exit_code = step->exit_code;
}
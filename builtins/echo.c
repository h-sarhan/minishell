/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:45:38 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/15 12:14:11 by mkhan            ###   ########.fr       */
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
			printf("%s", step->cmd->arg_arr[i]);
				if (step->cmd->arg_arr[i + 1])
					printf(" ");
		}
		if (ft_check(step->cmd->arg_arr[i]) != true && !seen)
		{
			seen = 1;
			printf("%s", step->cmd->arg_arr[i]);
			if (step->cmd->arg_arr[i + 1])
				printf(" ");
		}
		i++;
	}
	if (n == 0)
		printf("\n");
	step->exit_code = 0;
	shell->last_exit_code = step->exit_code;
}
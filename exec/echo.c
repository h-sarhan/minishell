/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:45:38 by mkhan             #+#    #+#             */
/*   Updated: 2022/08/29 21:56:39 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

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

void echo(t_exec_step *command)
{
	int	i;
	int n;
	int seen;
	
	i = 1;
	n = 0;
	seen = 0;
	if (ft_check(command->cmd->arg_arr[1]) == true)
		n = 1;
	while (command->cmd->arg_arr[i])
	{
		if (seen)
		{
			printf("%s", command->cmd->arg_arr[i]);
				if (command->cmd->arg_arr[i + 1])//|| *command->cmd->arg_arr[i] == '\0')
					printf(" ");
		}
		if (ft_check(command->cmd->arg_arr[i]) != true && !seen)
		{
			seen = 1;
			printf("%s", command->cmd->arg_arr[i]);
			if (command->cmd->arg_arr[i + 1])//|| *command->cmd->arg_arr[i] == '\0')
				printf(" ");
		}
		i++;
	}
	if (n == 0)
		printf("\n");
}
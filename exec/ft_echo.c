/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:45:38 by mkhan             #+#    #+#             */
/*   Updated: 2022/08/23 21:15:33 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

void ft_echo(t_exec_step *command)
{
	int	i;
	int n;
	
	i = 1;
	n = 0;
	printf("where do I seg fault \n");
	printf("%s\n", command->cmd->args);
	while (command->cmd->args->content && ft_strcmp(command->cmd->args->content, "-n") == 0)
	{
		printf("is it here? \n");
		// n = 1;
		command->cmd->args = command->cmd->args->next;
	 printf("%s", command->cmd->args->content);
	}
	// while (command->cmd->args->content)
	// {
	// 	printf("%s", command->cmd->args->content);
	// 	command->cmd->args = command->cmd->args->next;
	// }
	// if (n == 0)
	// 	write(1, "\n", 1);
}
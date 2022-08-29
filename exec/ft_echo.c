/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:45:38 by mkhan             #+#    #+#             */
/*   Updated: 2022/08/29 15:49:46 by mkhan            ###   ########.fr       */
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

// void ft_echo(t_exec_step *command)
// {
// 	t_list *start;
// 	int	i;
// 	int n;
	
// 	i = 1;
// 	n = 0;
// 	start = command->cmd->args->next;
// 	// printf("where do I seg fault \n");
// 	while (command->cmd->args->next && ft_strcmp(command->cmd->args->next->content, "-n") == 0)
// 	{
// 		printf("is it here? \n");
// 		n = 1;
// 	 	// printf("%s", command->cmd->args->content);
// 		command->cmd->args = command->cmd->args->next;
// 	}
// 	command->cmd->args = start;
// 	while (command->cmd->args)
// 	{
// 		printf("%s", command->cmd->args->content);
// 		command->cmd->args = command->cmd->args->next;
// 	}
// 	// if (n == 0)
// 	// 	write(1, "\n", 1);
// }
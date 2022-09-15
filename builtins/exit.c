/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 13:43:52 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/15 12:09:57 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit(t_exec_step *step, t_shell *shell, bool child)
{
	int	i;
	int j;
	int exitcode;
	bool	check;
	(void)shell;
	i = 0;
	j = 0;
	exitcode = 0;
	check = true;
	if (step->cmd->arg_arr[1] && (step->cmd->arg_arr[1][i] == '-' || step->cmd->arg_arr[1][i] == '+'))
		++i;
	while (step->cmd->arg_arr[1] && step->cmd->arg_arr[1][i] >= '0' && step->cmd->arg_arr[1][i] <= '9')
		++i;
	if ((step->cmd->arg_arr[1] && step->cmd->arg_arr[1][i]) || (step->cmd->arg_arr[1] && step->cmd->arg_arr[1][0] == '\0'))
	{
		ft_stderr("minishell: exit: %s: numeric argument required\n", step->cmd->arg_arr[1]);
		step->exit_code = 255;
		j = 1;
	}
	else if (step->cmd->arg_arr[1] && step->cmd->arg_arr[2] != NULL)
	{
		ft_stderr("minishell: exit: too many arguments\n");
		step->exit_code = 1;
		j = 1;
	}
	if (!j && step->cmd->arg_arr[1] != NULL)
		step->exit_code =  ft_atol(step->cmd->arg_arr[1], &check);
	if (!child)
		printf("exit\n");
	if (check == false)
	{
		step->exit_code = 255;
		ft_stderr("minishell: exit: %s: numeric argument required\n", step->cmd->arg_arr[1]);
	}
	shell->last_exit_code = step->exit_code;
	// exitcode = step->exit_code;
	// if (step->cmd->arg_arr)
	// {
	// 	ft_lstclear(&shell->tokens, free_token);
	// 	ft_lstclear(&shell->steps, free_exec_step);
	// 	free_split_array(shell->env);
	// 	// ft_free(&line);
	// }
	// printf("exit\n");
	// step->exit_code = exitcode;
	// exit(exitcode);
}
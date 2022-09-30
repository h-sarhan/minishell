/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 13:43:52 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_exitcode(t_shell *shell, t_exec_step *step, const int exitcode)
{
	step->exit_code = exitcode;
	shell->last_exit_code = step->exit_code;
}

/**
 * @brief Checks for valid args in exit. 
 * Mainly two cases.
 * 1) If the exit is called with arguments which are not numeric
 * 2) If exit has more than expected number of arguments.
 * 
 * @param step 
 * @param i 
 * @param j 
 */
void	check_valid_args(t_shell *shell, t_exec_step *step, int i, int *j)
{
	if ((step->cmd->arg_arr[1] && step->cmd->arg_arr[1][i])
		|| (step->cmd->arg_arr[1] && step->cmd->arg_arr[1][0] == '\0'))
	{
		ft_stderr("minishell: exit: %s: numeric argument required\n",
			step->cmd->arg_arr[1]);
		set_exitcode(shell, step, 255);
		*j = 1;
	}
	else if (step->cmd->arg_arr[1] && step->cmd->arg_arr[2] != NULL)
	{
		ft_stderr("minishell: exit: too many arguments\n");
		set_exitcode(shell, step, 1);
		*j = 1;
	}
}

/**
 * @brief Builtin Function exit. Checks for valid args and sets,
 * the exit code to the last exit code.
 * 
 * @param step 
 * @param shell 
 * @param child 
 */
void	ft_exit(t_exec_step *step, t_shell *shell, bool child)
{
	int		i;
	int		j;
	bool	check;

	i = 0;
	j = 0;
	check = true;
	if (step->cmd->arg_arr[1] && (step->cmd->arg_arr[1][i] == '-'
		|| step->cmd->arg_arr[1][i] == '+'))
		i++;
	while (step->cmd->arg_arr[1] && step->cmd->arg_arr[1][i] >= '0'
			&& step->cmd->arg_arr[1][i] <= '9')
		i++;
	check_valid_args(shell, step, i, &j);
	if (!j && step->cmd->arg_arr[1] != NULL)
		set_exitcode(shell, step, ft_atol(step->cmd->arg_arr[1], &check));
	if (check == false)
	{
		set_exitcode(shell, step, 255);
		ft_stderr("minishell: exit: %s: numeric argument required\n",
			step->cmd->arg_arr[1]);
	}
	if (!child)
		printf("exit\n");
}

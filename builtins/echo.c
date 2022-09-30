/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 17:45:38 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check for all cases of echo that handle -n.
 * if any case fails return false indicating to print a new line.
 * Else true to not print a new line.
 * 
 * @param s1 
 * @return true 
 * @return false 
 */
bool	ft_check(char *s1)
{
	int	i;

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

/**
 * @brief Function to print echo args and spaces if present.
 * 
 * @param step 
 * @param i 
 */
void	write_echo_content(t_exec_step *step, int i)
{
	ft_putstr_fd(step->cmd->arg_arr[i], 1);
	if (step->cmd->arg_arr[i + 1])
		ft_putstr_fd(" ", 1);
}

/**
 * @brief The function is triggered when echo is called,
 * calls the function that print the arguments, and
 * check for the validation of a new line.
 * 
 * @param step 
 * @param shell 
 */
void	ft_echo(t_exec_step *step, t_shell *shell)
{
	int	i;
	int	n;
	int	seen;

	i = 1;
	n = 0;
	seen = 0;
	if (ft_check(step->cmd->arg_arr[1]) == true)
		n = 1;
	while (step->cmd->arg_arr[i])
	{
		if (seen)
			write_echo_content(step, i);
		if (ft_check(step->cmd->arg_arr[i]) != true && !seen)
		{
			seen = 1;
			write_echo_content(step, i);
		}
		i++;
	}
	if (n == 0)
		ft_putstr_fd("\n", 1);
	step->exit_code = 0;
	shell->last_exit_code = step->exit_code;
}

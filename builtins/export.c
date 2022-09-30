/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 21:25:48 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(t_shell *shell, const char *str)
{
	size_t	i;
	char	*key;
	char	*val;
	char	*key_val;
	char	*to_look;

	i = 0;
	while (str[i] != '=')
		i++;
	key = ft_substr(str, 0, i);
	unset_declared_var(shell, key);
	val = ft_substr(str, i + 1, ft_strlen(str));
	key_val = ft_strjoin(key, "=");
	to_look = ft_strjoin(key, "=");
	key_val = strjoin_free(key_val, val, 3);
	i = 0;
	if (assign_val_to_env(shell, to_look, key, key_val) == false)
		return ;
	ft_free(&to_look);
	ft_free(&key);
	shell->env = resize_str_arr(shell->env, env_len(shell->env) + 1);
	shell->env[env_len(shell->env)] = key_val;
}

bool	check_export_arg(const char *arg)
{
	size_t	i;

	if (ft_isdigit(arg[0]) || arg[0] == '='
		|| (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (false);
	i = 1;
	while (arg[i] != '=' && arg[i] != '\0')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			return (false);
		}
		i++;
	}
	return (true);
}

static void	export_no_args(t_shell *shell, t_exec_step *step)
{
	size_t	i;
	int		j;
	char	*key;
	char	*val;

	i = 0;
	while (shell->env[i] != NULL)
	{
		j = 0;
		while (shell->env[i][j] != '=')
			j++;
		key = ft_substr(shell->env[i], 0, j);
		val = ft_substr(shell->env[i], j + 1, ft_strlen(shell->env[i]));
		print_the_export_env(&val, &key);
		ft_free(&key);
		i++;
	}
	step->exit_code = 0;
	shell->last_exit_code = step->exit_code;
	i = 0;
	while (shell->declared_env != NULL && shell->declared_env[i] != NULL)
	{
		printf("declare -x %s\n", shell->declared_env[i]);
		i++;
	}
}

void	update_declared_env(t_shell *shell, char *str)
{
	size_t	i;
	char	*to_look;

	i = -1;
	to_look = ft_strjoin(str, "=");
	while (shell->env[++i] != NULL)
	{
		if (ft_strncmp(shell->env[i], to_look, ft_strlen(to_look)) == 0)
		{
			ft_free(&to_look);
			return ;
		}
	}
	ft_free(&to_look);
	if (shell->declared_env == NULL)
	{
		shell->declared_env = ft_calloc(2, sizeof(char *));
		if (shell->declared_env == NULL)
			exit(EXIT_FAILURE);
		shell->declared_env[0] = ft_strdup(str);
		return ;
	}
	shell->declared_env = resize_str_arr(shell->declared_env,
			env_len(shell->declared_env) + 1);
	shell->declared_env[env_len(shell->declared_env)] = ft_strdup(str);
}

// // * Case 1: exported variable doesnt exist. Add new line
// // * Case 2: exported variable exists. Update variable
// // * Case 3: no equal sign. Do nothing
// // * Case 4: variable equals nothing. Set the value of the 
// variable to be nothing
// // * Case 5: variable equals "". Set the value of the variable to be nothing
// // * Case 6: Multiple variables. Update/set all of them
// // * Case 7: Includes = by itself. Show error but still update 
// other environmental variables if they are present
// // * Case 8: Identifier starts with a number. Show error but 
// still update other environmental variables if they are present

/**
 * @brief Function is triggered when export is called.
 * 
 * @param shell 
 * @param step 
 */
void	ft_export(t_shell *shell, t_exec_step *step)
{
	char	**args;
	bool	error;

	args = step->cmd->arg_arr;
	error = false;
	if (args[1] == NULL)
		export_no_args(shell, step);
	run_export_with_args(args, shell, &error);
	if (error)
		step->exit_code = 1;
	else
		step->exit_code = 0;
	shell->last_exit_code = step->exit_code;
}

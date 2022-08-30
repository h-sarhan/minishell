/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 21:25:48 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/31 00:35:03 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**resize_str_arr(char **old, size_t new_size)
{
	size_t	i;
	char	**new_arr;

	new_arr = ft_calloc(new_size + 1, sizeof(char *));
	if (new_arr == NULL)
	{
		// ! Handle error here
	}
	i = 0;
	while (old[i] != NULL)
	{
		new_arr[i] = old[i];
		i++;
	}
	ft_free(&old);
	return (new_arr);
}

size_t	env_len(char **env)
{
	size_t	len;

	len = 0;
	while (env[len] != NULL)
		len++;
	return (len);
}

void	update_env(t_shell *shell, const char *str)
{
	size_t	i;
	char	*key;
	char	*val;
	char	*key_val;

	i = 0;
	while (str[i] != '=')
		i++;
	key = ft_substr(str, 0, i);
	val = ft_substr(str, i + 1, ft_strlen(str));
	key_val = ft_strjoin(key, "=");
	key_val = strjoin_free(key_val, val, 3);
	i = 0;
	while (shell->env[i] != NULL)
	{
		if (ft_strncmp(shell->env[i], key, ft_strlen(key)) == 0)
		{
			ft_free(&shell->env[i]);
			ft_free(&key);
			shell->env[i] = key_val;
			return ;
		}
		i++;
	}
	ft_free(&key);
	shell->env = resize_str_arr(shell->env, env_len(shell->env) + 1);
	shell->env[env_len(shell->env)] = key_val;
}

// * Case 1: exported variable doesnt exist. Add new line
// * Case 2: exported variable exists. Update variable
// * Case 3: no equal sign. Do nothing
// * Case 4: variable equals nothing. Set the value of the variable to be nothing
// * Case 5: variable equals "". Set the value of the variable to be nothing
// * Case 6: Multiple variables. Update/set all of them
// * Case 7: Includes = by itself. Show error but still update other environmental variables if they are present
void	ft_export(t_shell *shell, const t_exec_step *step)
{
	char	**args;
	size_t	i;

	args = step->cmd->arg_arr;
	i = 0;
	while (args[i] != NULL)
	{
		// if the argument includes an equal sign
		if (ft_strchr(args[i], '=') != NULL)
			update_env(shell, args[i]);
		if (args[i][0] == '=')
			write(STDERR_FILENO, "export: Invalid identifier\n",
				ft_strlen("export: Invalid identifier\n"));
		i++;
	}
}

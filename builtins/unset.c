/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 19:01:46 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/19 11:44:38 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	unset_error(char *arg)
{
	ft_putstr_fd("unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("`: not a valid identifier", STDERR_FILENO);
}

static bool	check_unset_arg(const char *arg)
{
	size_t	i;

	if (ft_strchr(arg, '=') != NULL || ft_isdigit(arg[0]))
		return (false);
	i = 0;
	while (arg[i] != '\0')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			return (false);
		}
		i++;
	}
	return (true);
}

void	unset_var(t_shell *shell, const char *var)
{
	size_t	i;
	size_t	j;
	size_t	len;
	bool	found;
	char	*to_look;
	char	**env_copy;

	i = 0;
	found = false;
	to_look = ft_strjoin(var, "=");
	while (shell->env[i] != NULL)
	{
		if (ft_strncmp(shell->env[i], to_look, ft_strlen(to_look)) == 0)
			found = true;
		i++;
	}
	if (found == false)
	{
		ft_free(&to_look);
		return ;
	}
	len = env_len(shell->env) - 1;
	i = 0;
	j = 0;
	env_copy = ft_calloc(len + 1, sizeof(char *));
	while (shell->env[i] != NULL)
	{
		if (ft_strncmp(shell->env[i], to_look, ft_strlen(to_look)) != 0)
		{
			env_copy[j] = shell->env[i];
			i++;
			j++;
		}
		else
		{
			ft_free(&shell->env[i]);
			i++;
		}
	}
	ft_free(&shell->env);
	ft_free(&to_look);
	shell->env = env_copy;
	
}

void	unset_declared_var(t_shell *shell, const char *var)
{
	size_t	i;
	size_t	j;
	size_t	len;
	bool	found;
	char	*to_look;
	char	**env_copy;

	i = 0;
	found = false;
	if (shell->declared_env == NULL)
	{
		return ;
	}
	to_look = ft_strdup(var);
	while (shell->declared_env[i] != NULL)
	{
		if (ft_strncmp(shell->declared_env[i], to_look, ft_strlen(to_look)) == 0)
			found = true;
		i++;
	}
	if (found == false)
	{
		ft_free(&to_look);
		return ;
	}
	len = env_len(shell->declared_env) - 1;
	i = 0;
	j = 0;
	env_copy = ft_calloc(len + 1, sizeof(char *));
	while (shell->declared_env[i] != NULL)
	{
		if (ft_strncmp(shell->declared_env[i], to_look, ft_strlen(to_look)) != 0)
		{
			env_copy[j] = shell->declared_env[i];
			i++;
			j++;
		}
		else
		{
			ft_free(&shell->declared_env[i]);
			i++;
		}
	}
	ft_free(&shell->declared_env);
	ft_free(&to_look);
	shell->declared_env = env_copy;
}


void	ft_unset(t_shell *shell, t_exec_step *step)
{
	size_t	i;
	char	**args;
	bool	error;

	error = false;
	args = step->cmd->arg_arr;
	i = 1;
	while (args[i] != NULL)
	{
		// if the argument includes an equal sign
		if (check_unset_arg(args[i]) == false)
		{
			unset_error(args[i]);
			error = true;
		}
		else
		{
			unset_declared_var(shell, args[i]);
			unset_var(shell, args[i]);
		}
		i++;
	}
	if (error)
		step->exit_code = 1;
	else
		step->exit_code = 0;
	shell->last_exit_code = step->exit_code;
}


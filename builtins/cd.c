/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:41:46 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/26 14:53:10 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Find the $PWD env variable and update it with the path
 * of the current directory.
 * 
 * @param env 
 * @param shell 
 */
void	find_and_update_pwd(char **env, t_shell *shell)
{
	char	*pwd;
	int		i;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		pwd = get_env(shell, "PWD");
	if (!pwd)
		return ;
	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp("PWD=", env[i], 4))
		{
			ft_free(&env[i]);
			env[i] = ft_strjoin("PWD=", pwd);
			break ;
		}
	}
	ft_free(&pwd);
}

/**
 * @brief Find the $OLDPWD env variable and update it with the path
 * of the previous directory visited.
 * 
 * @param shell 
 * @param env 
 * @param oldpwd 
 */
void	find_and_update_oldpwd(t_shell *shell, char **env, char *oldpwd)
{
	int		i;
	char	*create_oldpwd;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp("OLDPWD=", env[i], 7))
		{
			ft_free(&env[i]);
			env[i] = ft_strjoin("OLDPWD=", oldpwd);
			break ;
		}
	}
	if (env[i] == NULL)
	{
		create_oldpwd = ft_strjoin("OLDPWD=", oldpwd);
		update_env(shell, create_oldpwd);
		ft_free(&create_oldpwd);
	}
}

/**
 * @brief Change the path and directory to the required directory.
 * Update the $PWD and $OLDPWD env.
 * 
 * @param shell 
 * @param step 
 * @param env 
 */
void	cd_to_path(t_shell *shell, t_exec_step *step, char **env)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		oldpwd = get_env(shell, "PWD");
	if (!oldpwd)
		return ;
	if (!chdir(step->cmd->arg_arr[1]) && ft_strlen(oldpwd))
	{
		find_and_update_pwd(env, shell);
		find_and_update_oldpwd(shell, env, oldpwd);
		step->exit_code = 0;
	}
	else if (chdir(step->cmd->arg_arr[1]) == -1)
	{
		ft_stderr("minishell: cd: %s: %s\n", step->cmd->arg_arr[1],
			strerror(errno));
		step->exit_code = 1;
	}
	ft_free(&oldpwd);
}

/**
 * @brief Change the path and directory to the home/root directory.
 * Update the $PWD and $OLDPWD env.
 * 
 * @param shell 
 * @param step 
 * @param env 
 * @param home 
 */
void	cd_to_home(t_shell *shell, t_exec_step *step, char **env, char *home)
{
	int		i;

	i = -1;
	while (env[++i])
		if (!ft_strncmp("HOME=", env[i], 5))
			home = ft_substr(env[i], 5, ft_strlen(env[1]));
	if (home)
		move_to_home(shell, step, env, home);
	else
	{
		ft_stderr("minishell: cd: HOME not set\n");
		step->exit_code = 1;
	}
}

/**
 * @brief The function is triggered when cd is called,
 * Calls he functions based on whether to change directory to
 * home or to the argument given.
 * 
 * @param step 
 * @param env 
 * @param shell 
 */
void	ft_cd(t_exec_step *step, char **env, t_shell *shell)
{
	char	*home;

	if (step->cmd->arg_arr[1])
		cd_to_path(shell, step, env);
	else
	{
		home = NULL;
		cd_to_home(shell, step, env, home);
	}
	shell->last_exit_code = step->exit_code;
}

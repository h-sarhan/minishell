/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:41:46 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/01 13:14:27 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	find_and_update_pwd(char **env)
{
	char	*pwd;
	int		i;
	
	pwd = getcwd(NULL, 0);
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

void	find_and_update_oldpwd(char **env, char *oldpwd)
{
	int	i;

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
}

void	cd_to_path(t_exec_step *step, char **env)
{
	char	*oldpwd;
	
	oldpwd = getcwd(NULL, 0);
	if (!chdir(step->cmd->arg_arr[1]) && ft_strlen(oldpwd))
	{
		find_and_update_pwd(env);
		find_and_update_oldpwd(env, oldpwd);
		step->exit_code = 0;
	}
	else if (chdir(step->cmd->arg_arr[1]) == -1)
	{
		ft_stderr("minishell: cd: %s: %s\n", step->cmd->arg_arr[1], strerror(errno));
				
		step->exit_code = 1;
	}
	ft_free(&oldpwd);
}

void	cd_to_home(t_exec_step *step, char **env, char *home)
{
	char	*oldpwd;
	int		i;
	
	i = -1;
	while (env[++i])
		if (!ft_strncmp("HOME=", env[i], 5))
			home = ft_substr(env[i], 5, ft_strlen(env[1]));
	if (home)
	{
		oldpwd = getcwd(NULL, 0);
		if (!chdir(home) && ft_strlen(oldpwd))
		{
			find_and_update_pwd(env);
			find_and_update_oldpwd(env, oldpwd);
			step->exit_code = 0;
		}
		ft_free(&oldpwd);
		ft_free(&home);
	}
	else
	{
		ft_stderr("minishell: cd: HOME not set\n");
		step->exit_code = 1;
	}
}

void	ft_cd(t_exec_step *step, char **env)
{
	char	*home;
	
	if (step->cmd->arg_arr[1])
		cd_to_path(step, env);
	else
	{
		home = NULL;
		cd_to_home(step, env, home);
	}
}

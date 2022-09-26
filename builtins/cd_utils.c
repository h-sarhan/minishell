/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 14:43:16 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/26 14:54:08 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief move from the current directory to the home/root directory.
 * 
 * @param shell 
 * @param step 
 * @param env 
 * @param home 
 */
void	move_to_home(t_shell *shell, t_exec_step *step, char **env, char *home)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		oldpwd = get_env(shell, "PWD");
		ft_free(&home);
		return ;
	}
	if (!chdir(home) && ft_strlen(oldpwd))
	{
		find_and_update_pwd(env, shell);
		find_and_update_oldpwd(shell, env, oldpwd);
		step->exit_code = 0;
	}
	ft_free(&oldpwd);
	ft_free(&home);
}

/**
 * @brief Iterates through the env variables, 
 * and returns the length of them.
 * 
 * @param env 
 * @return size_t 
 */
size_t	env_len(char **env)
{
	size_t	len;

	len = 0;
	while (env[len] != NULL)
		len++;
	return (len);
}

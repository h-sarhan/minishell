/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 17:43:57 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/30 21:25:25 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_env(const t_shell *shell, t_exec_step *step)
{
	size_t	i;

	i = 0;
	if (step->cmd->arg_arr[1] != NULL)
	{
		step->exit_code = 1;
		return ;
	}
	while (shell->env[i] != NULL)
	{
		printf("%s\n", shell->env[i]);
		i++;
	}
	step->exit_code = 0;
}

char	*get_env(const t_shell *shell, const char *name)
{
	size_t		i;
	char	**vars;
	char	*look_for;
	size_t	look_for_len;

	i = 0;
	vars = shell->env;
	while (vars[i] != NULL)
	{
		look_for = ft_strjoin(name, "=");
		look_for_len = ft_strlen(look_for);
		if (ft_strncmp(vars[i], look_for, look_for_len) == 0)
		{
			ft_free(&look_for);
			return (ft_substr(vars[i], look_for_len, ft_strlen(vars[i]) - look_for_len + 1));
		}
		ft_free(&look_for);
		i++;
	}
	return (NULL);
}
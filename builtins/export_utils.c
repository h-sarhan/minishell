/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 14:37:20 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/26 14:46:13 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_error(char *arg)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("`: not a valid identifier", STDERR_FILENO);
}

char	**resize_str_arr(char **old, size_t new_size)
{
	size_t	i;
	char	**new_arr;

	new_arr = ft_calloc(new_size + 1, sizeof(char *));
	if (new_arr == NULL)
		exit(1);
	i = 0;
	while (old[i] != NULL)
	{
		new_arr[i] = old[i];
		i++;
	}
	ft_free(&old);
	return (new_arr);
}

bool	assign_val_to_env(t_shell *shell, char *to_look,
			char *key, char *key_val)
{
	int	i;

	i = 0;
	while (shell->env[i] != NULL)
	{
		if (ft_strncmp(shell->env[i], to_look, ft_strlen(to_look)) == 0)
		{
			ft_free(&to_look);
			ft_free(&shell->env[i]);
			ft_free(&key);
			shell->env[i] = key_val;
			return (false);
		}
		i++;
	}
	return (true);
}

void	print_the_export_env(char **val, char **key)
{
	if (*val[0] == '\0')
	{
		ft_free(val);
		printf("declare -x %s\n", *key);
	}
	else
	{
		*val = strjoin_free("=\"", *val, 2);
		*val = strjoin_free(*val, "\"", 1);
		*key = strjoin_free(*key, *val, 3);
		printf("declare -x %s\n", *key);
	}
}

void	run_export_with_args(char **args, t_shell *shell, bool *error)
{
	int	i;

	i = 1;
	while (args[i] != NULL)
	{
		if (check_export_arg(args[i]) == false)
		{
			*error = true;
			export_error(args[i]);
		}
		else if (ft_strchr(args[i], '=') != NULL)
			update_env(shell, args[i]);
		else
		{
			unset_declared_var(shell, args[i]);
			update_declared_env(shell, args[i]);
		}
		i++;
	}
}

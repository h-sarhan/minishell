/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 14:20:28 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the dir/path to the directory exists.
 * 
 * @param path 
 * @return int 
 */
int	is_dir(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

/**
 * @brief A wrapper functions which calls the clos function.
 * It closes the fds if its != -1 (Invalid Fds).
 * 
 * @param fd 
 */
void	ft_close(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

/**
 * @brief Joins the path to the command.
 * 
 * @param bin 
 * @param paths 
 * @param path 
 */
static void	join_path(char *bin, char **paths, char **path)
{
	int	i;

	i = 0;
	while (bin != NULL && paths != NULL && paths[i] != NULL)
	{
		*path = ft_strjoin(paths[i], bin);
		if (*path == NULL || access(*path, X_OK) != -1)
			break ;
		ft_free(path);
		i++;
	}
}

/**
 * @brief Get the full path of a command.
 * 
 * @param bin 
 * @param env 
 * @return char* 
 */
char	*get_full_path(char *bin, char **env)
{
	int		i;
	char	*path;
	char	**paths;
	char	*bin_cpy;

	i = 0;
	while (env[i] != NULL && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i] == NULL)
		return (NULL);
	paths = ft_split(ft_strchr(env[i], '=') + 1, ':');
	i = 0;
	bin_cpy = ft_strdup(bin);
	bin = strjoin_free("/", bin, 2);
	join_path(bin, paths, &path);
	if (bin == NULL || paths == NULL || paths[i] == NULL)
		path = NULL;
	ft_free(&bin);
	free_split_array(paths);
	if (path == NULL)
		return (bin_cpy);
	ft_free(&bin_cpy);
	return (path);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:02:17 by hsarhan           #+#    #+#             */
/*   Updated: 2022/07/04 12:03:47 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// This is ft_strjoin but with an additional argument
// to specify which input strings to free
char	*ft_strjoinfree(char *s1, char *s2, int f)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	if (f == 1)
		ft_free(s1);
	if (f == 2)
		ft_free(s2);
	if (f == 3)
	{
		ft_free(s1);
		ft_free(s2);
	}
	return (joined);
}

// Frees an array created by ft_split (or split_args)
void	free_split_array(char **arr)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		ft_free(arr[i]);
		i++;
	}
	ft_free(arr);
}

// Trims the quotation marks from a list of arguments
char	**trim_args(char **args)
{
	int		i;
	char	*trimmed_arg;
	char	*quote;

	i = 0;
	while (args[i] != NULL)
	{
		if (args[i][0] == '\'' || args[i][0] == '\"')
		{
			quote = ft_calloc(2, sizeof(char));
			if (quote == NULL)
				return (NULL);
			quote[0] = args[i][0];
			quote[1] = '\0';
			trimmed_arg = ft_strtrim(args[i], quote);
			ft_free(args[i]);
			ft_free(quote);
			if (trimmed_arg == NULL)
				return (NULL);
			args[i] = trimmed_arg;
		}
		i++;
	}
	return (args);
}

// Tries to get the full path of a command
// Returns NULL on failure
char	*get_full_path(char *bin, char **env)
{
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	while (env[i] != NULL && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i] == NULL)
		return (NULL);
	paths = ft_split(ft_strchr(env[i], '=') + 1, ':');
	i = 0;
	bin = ft_strjoinfree("/", bin, 2);
	while (bin != NULL && paths != NULL && paths[i] != NULL)
	{
		path = ft_strjoin(paths[i], bin);
		if (path == NULL || access(path, X_OK) != -1)
			break ;
		ft_free(path);
		i++;
	}
	if (bin == NULL || paths == NULL || paths[i] == NULL)
		path = NULL;
	ft_free(bin);
	free_split_array(paths);
	return (path);
}

// Prints an error message and exits if the number of arguments is invalid
// or if any of the arguments are empty
void	check_args(int argc, char **argv)
{
	int	i;

	if (argc < 5)
	{
		ft_putendl_fd("Wrong number of arguments", 2);
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0 && argc < 6)
	{
		ft_putendl_fd("Wrong number of arguments", 2);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (argv[i] != NULL)
	{
		if (argv[i][0] == '\0')
		{
			ft_putendl_fd("Invalid argument", 2);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

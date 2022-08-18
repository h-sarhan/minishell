/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 07:59:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/18 08:18:26 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// getcwd:
// Returns the absolute path to the current working directory. Need this for cd.
// stat/fstat/lstat:
// Useful for finding out the permissions of a file and whether or not it is a directory.
// unlink:
// Deletes files. Might be needed for the heredoc.
// opendir/readdir/closedir:
// Use these to scan through files in a directory. Might be needed for wildcard bonus.

char	*get_dir_contents(void)
{
	char			*contents;
	DIR				*dp;
	struct dirent	*dirp;
	char			*cwd;
	
	cwd = getcwd(NULL, 0);
	dp = opendir(cwd);
	free(cwd);

	contents = ft_strdup("");
	while ((dirp = readdir(dp)) != NULL)
	{
		if (dirp->d_name[0] != '.')
		{
			contents = strjoin_free(contents, " ", 1);
			contents = strjoin_free(contents, dirp->d_name, 1);
		}
	}
	closedir(dp);
	return (contents);
}

char	*expand_wildcard(char *token)
{
	if (ft_strncmp(token, "*", ft_strlen("*")) == 0)
	{
		return (get_dir_contents());
	}
	return (NULL);
}
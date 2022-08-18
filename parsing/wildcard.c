/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 07:59:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/18 10:41:17 by hsarhan          ###   ########.fr       */
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

// ! Do better error handling here
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
			if (contents[0] != '\0')
				contents = strjoin_free(contents, " ", 1);
			contents = strjoin_free(contents, dirp->d_name, 1);
		}
	}
	closedir(dp);
	return (contents);
}

// static bool	match_str_on_wildcard(char *str, char **wc_segs)
// {
// 	size_t	i;
// 	size_t	wc_i;
// 	char	*curr;

// 	i = 0;
// 	wc_i = 0;
// 	curr = wc_segs[wc_i];
// 	while (str[i] != '\0' && wc_segs[wc_i] != NULL)
// 	{
// 		if (curr[0] == '*')
// 		{
// 			while (str[i] != '\0' && ft_strncmp(&str[i], wc_segs[wc_i + 1], ft_strlen(wc_segs[wc_i + 1])) != 0)
// 				i++;
// 			if (str[i] == '\0')
// 				break;
// 			i += ft_strlen(wc_segs[wc_i + 1]);
// 			if (wc_segs[wc_i + 2] == NULL)
// 				break;
// 			wc_i += 2;
// 		}
// 		else
// 		{
// 			if (ft_strncmp(&str[i], curr, ft_strlen(curr)) == 0)
// 				i += ft_strlen(curr);
// 			else
// 				return (false);
// 			if (wc_segs[wc_i + 1] == NULL)
// 				break;
// 			wc_i++;
// 		}
// 		curr = wc_segs[wc_i];
// 	}
// 	return (true);
// }

// ! Do better error handling here
char	*expand_wildcard(char *token)
{
	// char	**contents;
	// char	*contents_str;
	// char	**split_wc;
	// size_t	i;

	if (ft_strncmp(token, "*", ft_strlen(token)) == 0)
		return (get_dir_contents());
	// else
	// {
	// 	contents_str = get_dir_contents();
	// 	contents = ft_split(contents_str, ' ');
	// 	ft_free(&contents_str);
	// 	i = 0;
	// 	printf("wildcard token is %s\n", token);
	// 	split_wc = split_wildcard(token);
	// 	while (contents[i] != NULL)
	// 	{
	// 		if (match_str_on_wildcard(contents[i], split_wc))
	// 			printf("matched on %s\n", contents[i]);
	// 		i++;
	// 	}
	// }
	return (NULL);
}

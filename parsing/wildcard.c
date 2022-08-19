/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 07:59:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/19 09:38:12 by hsarhan          ###   ########.fr       */
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

static bool	match_str_on_wildcard(char *str, char **wc_segs)
{
	size_t	i;
	size_t	wc_i;

	i = 0;
	wc_i = 0;

	while (str[i] != '\0' && wc_segs[wc_i] != NULL)
	{
		// example ne*
		if (wc_segs[wc_i][0] != '*' && wc_segs[wc_i + 1] != NULL)
		{
			if (ft_strncmp(&str[i], wc_segs[wc_i], ft_strlen(wc_segs[wc_i])) == 0)
			{
				i += ft_strlen(wc_segs[wc_i]);
				wc_i++;
				if (str[i] == '\0' && wc_segs[wc_i] == NULL)
				{
					// printf("%s DONE WITH CHAR SEQ THEN *\n", str);
					// printf("Returning true\n");
					return (true);
				}
			}
			else
			{
				// printf("%s DONE WITH CHAR SEQ THEN *\n", str);
				// printf("Returning false\n");
				return (false);
			}
			// printf("%s DONE WITH CHAR SEQ THEN *\n", str);
		}
		// example ne*ne
		else if (wc_segs[wc_i][0] == '*' && wc_segs[wc_i + 1] != NULL)
		{
			while (str[i] != '\0' && ft_strncmp(&str[i], wc_segs[wc_i + 1], ft_strlen(wc_segs[wc_i + 1])) != 0)
				i++;
			if (str[i] == '\0')
			{
				// printf("%s DONE WITH * THEN CHAR SEQ\n", str);
				// printf("Returning false\n");
				return (false);
			}
			wc_i++;
			// printf("%s DONE WITH * THEN CHAR SEQ\n", str);
		}
		// example ne*
		else if (wc_segs[wc_i][0] == '*' && wc_segs[wc_i + 1] == NULL)
			return (true);
		// example ne*n
		else if (wc_segs[wc_i][0] != '*' && wc_segs[wc_i + 1] == NULL)
		{
			// printf("%s %s\n", str, &str[i]);
			if (ft_strncmp(&str[i], wc_segs[wc_i], ft_strlen(wc_segs[wc_i])) == 0)
			{
				i += ft_strlen(wc_segs[wc_i]);
				if (str[i] == '\0')
				{
					// printf("%s DONE WITH CHAR SEQ THEN NULL\n", str);
					// printf("Returning true\n");
					return (true);
				}
				i++;
				wc_i--;
			}
			else
			{
				// printf("%s failed to match %s with %s\n", str, ft_substr(str, i, ft_strlen(wc_segs[wc_i])), wc_segs[wc_i]);
				// printf("%s DONE WITH CHAR SEQ THEN NULL\n", str);
				// return (false);
				i++;
			}
			// printf("%s DONE WITH CHAR SEQ THEN NULL\n", str);
		}
		// else
		// 	i++;
	}
	if (str[i] == '\0' && wc_segs[wc_i][0] == '*' && wc_segs[wc_i + 1] == NULL)
		return (true);
	return (false);
}

// ! Do better error handling here
char	*expand_wildcard(char *token)
{
	char	**contents;
	char	*contents_str;
	char	**split_wc;
	size_t	i;
	char	*res;

	if (ft_strncmp(token, "*", ft_strlen(token)) == 0)
	{
		ft_free(&token);
		return (get_dir_contents());
	}
	else
	{
		res = ft_strdup("");
		contents_str = get_dir_contents();
		contents = ft_split(contents_str, ' ');
		ft_free(&contents_str);
		i = 0;
		// printf("wildcard token is %s\n", token);
		split_wc = split_wildcard(token);
		ft_free(&token);
		while (contents[i] != NULL)
		{
			if (match_str_on_wildcard(contents[i], split_wc))
			{
				// printf("matched on %s\n", contents[i]);
				if (res[0] != '\0')
					res = strjoin_free(res, " ", 1);
				res = strjoin_free(res, contents[i], 1);
			}
			i++;
		}
		free_split_array(split_wc);
		free_split_array(contents);
	}
	return (res);
}

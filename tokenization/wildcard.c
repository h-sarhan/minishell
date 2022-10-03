/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 07:59:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/10/03 19:55:10 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_dir_contents(void)
{
	char			*contents;
	DIR				*dp;
	struct dirent	*dirp;
	char			*cwd;

	cwd = getcwd(NULL, 0);
	contents = ft_strdup("");
	if (cwd == NULL)
		return (ft_strdup(""));
	dp = opendir(cwd);
	ft_free(&cwd);
	if (dp == NULL)
		return (ft_strdup(""));
	dirp = readdir(dp);
	while (dirp != NULL)
	{
		if (dirp->d_name[0] != '.' && contents[0] != '\0')
			contents = strjoin_free(contents, "\n", 1);
		if (dirp->d_name[0] != '.')
			contents = strjoin_free(contents, dirp->d_name, 1);
		dirp = readdir(dp);
	}
	closedir(dp);
	return (contents);
}

static char	*single_wildcard(char *token)
{
	char	*res;

	res = get_dir_contents();
	if (*res == '\0')
		res = ft_strdup(token);
	while (ft_strchr(res, '\n') != NULL)
		ft_strchr(res, '\n')[0] = ' ';
	ft_free(&token);
	return (res);
}

static char	*expand_wildcard_cleanup(char *res, char *token, t_wildcard **split_wc,
		char **contents)
{
	if (ft_strlen(res) == 0)
	{
		ft_free(&res);
		res = ft_strdup(token);
	}
	ft_free(&token);
	// ! FREE THIS SOME OTHER WAY
	// free_split_array(split_wc);
	(void)split_wc;
	free_split_array(contents);
	return (eat_quotes(res));
}

char	*expand_wildcard(char *token)
{
	char		**contents;
	char		*contents_str;
	t_wildcard	**wildcards;
	int			i;
	char		*res;

	if (ft_strncmp(token, "*", ft_strlen(token)) == 0)
		return (single_wildcard(token));
	res = ft_strdup("");
	contents_str = get_dir_contents();
	contents = ft_split(contents_str, '\n');
	ft_free(&contents_str);
	wildcards = split_wildcard(token);
	bool	all_charseqs = true;
	i = 0;
	while (wildcards[i] != NULL)
	{
		if (wildcards[i]->is_wildcard == true)
			all_charseqs = false;
		i++;
	}
	if (all_charseqs == true)
		return (expand_wildcard_cleanup(res, token, wildcards, contents));
	i = -1;
	while (contents[++i] != NULL)
	{
		if (match_str_on_wildcard(contents[i], wildcards) == true
			&& res[0] != '\0')
			res = strjoin_free(res, " ", 1);
		if (match_str_on_wildcard(contents[i], wildcards) == true)
			res = strjoin_free(res, contents[i], 1);
	}
	return (expand_wildcard_cleanup(res, token, wildcards, contents));
}

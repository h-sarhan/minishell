/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 07:59:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/29 10:02:05 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			contents = strjoin_free(contents, " ", 1);
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
	ft_free(&token);
	return (res);
}

static char	*expand_wildcard_cleanup(char *res, char *token, char **split_wc,
		char **contents)
{
	if (ft_strlen(res) == 0)
	{
		ft_free(&res);
		res = ft_strdup(token);
	}
	ft_free(&token);
	free_split_array(split_wc);
	free_split_array(contents);
	return (res);
}

char	*expand_wildcard(char *token)
{
	char	**contents;
	char	*contents_str;
	char	**split_wc;
	size_t	i;
	char	*res;

	if (ft_strncmp(token, "*", ft_strlen(token)) == 0)
		return (single_wildcard(token));
	res = ft_strdup("");
	contents_str = get_dir_contents();
	contents = ft_split(contents_str, ' ');
	ft_free(&contents_str);
	i = -1;
	split_wc = split_wildcard(token);
	while (contents[++i] != NULL)
	{
		if (match_str_on_wildcard(contents[i], split_wc) == true
			&& res[0] != '\0')
			res = strjoin_free(res, " ", 1);
		if (match_str_on_wildcard(contents[i], split_wc) == true)
			res = strjoin_free(res, contents[i], 1);
	}
	return (expand_wildcard_cleanup(res, token, split_wc, contents));
}

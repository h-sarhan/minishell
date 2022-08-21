/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 07:59:27 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 12:15:35 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_dir_contents(void)
{
	char			*contents;
	DIR				*dp;
	struct dirent	*dirp;
	char			*cwd;

	cwd = getcwd(NULL, 0);
	dp = opendir(cwd);
	ft_free(&cwd);
	contents = ft_strdup("");
	dirp = readdir(dp);
	while (dirp != NULL)
	{
		if (dirp->d_name[0] != '.')
		{
			if (contents[0] != '\0')
				contents = strjoin_free(contents, " ", 1);
			contents = strjoin_free(contents, dirp->d_name, 1);
		}
		dirp = readdir(dp);
	}
	closedir(dp);
	return (contents);
}

static int	non_terminating_charseq(const char *str, size_t *wc_i, size_t *i,
								char **wc_segs)
{
	size_t	seg_length;

	seg_length = ft_strlen(wc_segs[*wc_i]);
	if (wc_segs[*wc_i][0] != '*' && wc_segs[*wc_i + 1] != NULL)
	{
		if (ft_strncmp(&str[*i], wc_segs[*wc_i], seg_length) == 0)
		{
			*i += seg_length;
			*wc_i += 1;
			if (str[*i] == '\0' && wc_segs[*wc_i] == NULL)
				return (TRUE);
			return (CONTINUE);
		}
		else
			return (FALSE);
	}
	return (CONTINUE);
}

static int	non_terminating_wildcard(const char *str, size_t *wc_i, size_t *i,
								char **wc_segs)
{
	size_t	seg_length;

	seg_length = ft_strlen(wc_segs[*wc_i + 1]);
	if (wc_segs[*wc_i][0] == '*' && wc_segs[*wc_i + 1] != NULL)
	{
		while (str[*i] != '\0'
			&& ft_strncmp(&str[*i], wc_segs[*wc_i + 1], seg_length) != 0)
			*i += 1;
		if (str[*i] == '\0')
			return (FALSE);
		*wc_i += 1;
	}
	return (CONTINUE);
}

static int	terminating_charseq(const char *str, size_t *wc_i, size_t *i,
								char **wc_segs)
{
	if (wc_segs[*wc_i][0] != '*' && wc_segs[*wc_i + 1] == NULL)
	{
		if (ft_strncmp(&str[*i], wc_segs[*wc_i],
				ft_strlen(wc_segs[*wc_i])) == 0)
		{
			*i += ft_strlen(wc_segs[*wc_i]);
			if (str[*i] == '\0')
				return (TRUE);
			*wc_i -= 1;
		}
		else
			i++;
	}
	return (CONTINUE);
}

static bool	match_str_on_wildcard(const char *str, char **wc_segs)
{
	size_t	i;
	size_t	wc_i;
	int		res;

	i = 0;
	wc_i = 0;
	while (str[i] != '\0' && wc_segs[wc_i] != NULL)
	{
		if (wc_segs[wc_i][0] != '*' && wc_segs[wc_i + 1] != NULL)
			res = non_terminating_charseq(str, &wc_i, &i, wc_segs);
		else if (wc_segs[wc_i][0] == '*' && wc_segs[wc_i + 1] != NULL)
			res = non_terminating_wildcard(str, &wc_i, &i, wc_segs);
		else if (wc_segs[wc_i][0] == '*' && wc_segs[wc_i + 1] == NULL)
			return (true);
		else if (wc_segs[wc_i][0] != '*' && wc_segs[wc_i + 1] == NULL)
			res = terminating_charseq(str, &wc_i, &i, wc_segs);
		if (res != CONTINUE)
			return (res);
	}
	if (str[i] == '\0' && wc_segs[wc_i][0] == '*' && wc_segs[wc_i + 1] == NULL)
		return (true);
	return (false);
}

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
		split_wc = split_wildcard(token);
		ft_free(&token);
		while (contents[i] != NULL)
		{
			if (match_str_on_wildcard(contents[i], split_wc) == true)
			{
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_algorithm.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 09:24:29 by hsarhan           #+#    #+#             */
/*   Updated: 2022/10/03 19:55:40 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	non_terminating_charseq(const char *str, size_t *wc_i, size_t *i,
								t_wildcard **wc_segs)
{
	size_t	seg_length;

	seg_length = ft_strlen(wc_segs[*wc_i]->str);
	if (wc_segs[*wc_i]->is_wildcard == false && wc_segs[*wc_i + 1] != NULL)
	{
		if (ft_strncmp(&str[*i], wc_segs[*wc_i]->str, seg_length) == 0)
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
								t_wildcard **wc_segs)
{
	size_t	seg_length;

	seg_length = ft_strlen(wc_segs[*wc_i + 1]->str);
	if (wc_segs[*wc_i]->is_wildcard == true && wc_segs[*wc_i + 1] != NULL)
	{
		while (str[*i] != '\0'
			&& ft_strncmp(&str[*i], wc_segs[*wc_i + 1]->str, seg_length) != 0)
			*i += 1;
		if (str[*i] == '\0')
			return (FALSE);
		*wc_i += 1;
	}
	return (CONTINUE);
}

static int	terminating_charseq(const char *str, size_t *wc_i, size_t *i,
								t_wildcard **wc_segs)
{
	if (wc_segs[*wc_i]->is_wildcard == false && wc_segs[*wc_i + 1] == NULL)
	{
		if (ft_strncmp(&str[*i], wc_segs[*wc_i]->str,
				ft_strlen(wc_segs[*wc_i]->str)) == 0)
		{
			*i += ft_strlen(wc_segs[*wc_i]->str);
			if (str[*i] == '\0')
				return (TRUE);
			*wc_i -= 1;
		}
		else
			i++;
	}
	return (CONTINUE);
}

bool	match_str_on_wildcard(const char *str, t_wildcard **wc_segs)
{
	size_t	i;
	size_t	wc_i;
	int		res;

	i = 0;
	wc_i = 0;
	while (str[i] != '\0' && wc_segs[wc_i] != NULL)
	{
		if (wc_segs[wc_i]->is_wildcard == false && wc_segs[wc_i + 1] != NULL)
		{
			res = non_terminating_charseq(str, &wc_i, &i, wc_segs);
		}
		else if (wc_segs[wc_i]->is_wildcard == true && wc_segs[wc_i + 1] != NULL)
		{
			res = non_terminating_wildcard(str, &wc_i, &i, wc_segs);
		}
		else if (wc_segs[wc_i]->is_wildcard == true && wc_segs[wc_i + 1] == NULL)
			return (true);
		else if (wc_segs[wc_i]->is_wildcard == false && wc_segs[wc_i + 1] == NULL)
		{
			res = terminating_charseq(str, &wc_i, &i, wc_segs);
		}
		if (res != CONTINUE)
			return (res);
	}
	if (str[i] == '\0' && wc_segs[wc_i]->is_wildcard == true && wc_segs[wc_i + 1] == NULL)
		return (true);
	return (false);
}

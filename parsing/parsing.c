/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:46:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/17 15:01:03 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	parse_line(const char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\\' || line[i] == ';')
		{
			write_to_stderr("Parse Error: Invalid operator\n");
			return (false);
		}
		i++;
	}
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/17 14:54:44 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	main(void)
{
	char	*line;
	while (1)
	{
		line = readline("minishell$ ");
		if (parse_line(line) == false)
		{
			free(line);
			exit(EXIT_FAILURE);
		}
		if (line == NULL)
		{
			printf("\n");
			return (EXIT_SUCCESS);
		}
		// printf("%s\n", line);
		free(line);
	}
}

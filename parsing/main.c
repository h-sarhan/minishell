/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/17 19:11:06 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	main(void)
{
	char	*line;
	bool	success;
	t_list	*tokens;

	success = true;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL || *line == '\0')
		{
			printf("\n");
			return (EXIT_SUCCESS);
		}
		tokens = parse_line(line, &success);
		if (success == false)
		{
			free(line);
			return (EXIT_FAILURE);
		}
		print_tokens(tokens);
		rl_on_new_line(); // I dont know what this does
		free(line);
	}
}

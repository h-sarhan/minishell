/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/18 08:15:17 by hsarhan          ###   ########.fr       */
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
		if (success == true)
			print_tokens(tokens);
		rl_on_new_line(); // I dont know what this does
		free(line);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/19 16:02:46 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// ? I dont know what rl_on_new_line() this does
int	main(void)
{
	char	*line;
	bool	success;
	t_list	*tokens;

	success = true;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
		{
			printf("\n");
			return (EXIT_SUCCESS);
		}
		if (line[0] != '\0')
			add_history(line);
		tokens = tokenize_line(line, &success);
		if (success == true)
		{
			print_tokens(tokens);
		}
		ft_lstclear(&tokens, free_token);
		rl_on_new_line();
		free(line);
	}
	clear_history();
}

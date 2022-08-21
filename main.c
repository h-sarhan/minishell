/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 13:09:41 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			char *expanded_line = join_tokens(tokens);
			ft_lstclear(&tokens, free_token);
			tokens = tokenize_line(expanded_line, &success);
			ft_free(&expanded_line);
		}
		t_list *exec_steps = parse_tokens(tokens);
		while (exec_steps != NULL)
		{
			t_exec_step	*step = exec_steps->content;
			char	**args = step->cmd->args;
			size_t	i = 0;
			while (args[i] != NULL)
			{
				printf("arg %lu: %s\n", i + 1, args[i]);
				i++;
			}
			printf("to be piped == %d\n", step->cmd->pipe);
			exec_steps = exec_steps->next;
		}
		ft_lstclear(&tokens, free_token);
		rl_on_new_line();
		free(line);
	}
	clear_history();
}

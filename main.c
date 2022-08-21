/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 21:53:45 by hsarhan          ###   ########.fr       */
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
		t_list *exec_steps = parse_tokens(tokens, &success);
		t_list *exec_steps_start = exec_steps;
		if (success == false)
		{
			write_to_stderr("Parse error\n");
			ft_lstclear(&tokens, free_token);
			ft_lstclear(&exec_steps_start, free_exec_step);
			rl_on_new_line();
			free(line);
			continue;
		}
		while (exec_steps != NULL)
		{
			t_exec_step	*step = exec_steps->content;
			t_list	*redirs = step->cmd->redirs;
			t_redir	*redir;
			t_list	*args = step->cmd->args;
			size_t	i = 0;
			while (args != NULL)
			{
				printf("Arg #%lu == %s\n", i + 1, (char *)args->content);
				args = args->next;
				i++;
			}
			printf("Pipe into next command == %d\n", step->pipe_next);
			while (redirs != NULL)
			{
				redir = redirs->content;
				if (redir->type == INPUT_REDIR)
					printf("Input redirection from %s\n", redir->file);
				if (redir->type == OUTPUT_REDIR)
					printf("Output redirection to %s\n", redir->file);
				if (redir->type == APPEND)
					printf("Append redirection to %s\n", redir->file);
				redirs = redirs->next;
			}
			exec_steps = exec_steps->next;
		}
		ft_lstclear(&tokens, free_token);
		ft_lstclear(&exec_steps_start, free_exec_step);
		rl_on_new_line();
		free(line);
	}
	clear_history();
}

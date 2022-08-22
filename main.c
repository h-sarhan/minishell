/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/22 07:20:27 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exec_step(t_list *exec_steps)
{
	t_exec_step	*step = exec_steps->content;
	t_redir	*redir;
	size_t	i = 0;
	t_list	*args = NULL;
	t_list	*redirs = NULL;
	
	if (step->cmd != NULL)
	{
		args = step->cmd->args;
		redirs = step->cmd->redirs;
	}
	if (step->subexpr_steps != NULL)
	{
		exec_steps = step->subexpr_steps;
		while (exec_steps != NULL)
		{
			print_exec_step(exec_steps);
			exec_steps = exec_steps->next;
		}
		printf("Pipe subexpr  into next command == %d\n", step->pipe_next);
		printf("AND  subexpr into next command == %d\n", step->and_next);
		printf("OR   subexpr into next command == %d\n", step->or_next);
		return ;
	}
	while (args != NULL)
	{
		printf("Arg #%lu == %s\n", i + 1, (char *)args->content);
		args = args->next;
		i++;
	}
	
	printf("Pipe into next command == %d\n", step->pipe_next);
	printf("AND into next command == %d\n", step->and_next);
	printf("OR into next command == %d\n", step->or_next);
	while (redirs != NULL)
	{
		redir = redirs->content;
		if (redir->type == INPUT_REDIR)
			printf("Input redirection from %s\n", redir->file);
		if (redir->type == OUTPUT_REDIR)
			printf("Output redirection to %s\n", redir->file);
		if (redir->type == APPEND)
			printf("Append redirection to %s\n", redir->file);
		if (redir->type == HEREDOC)
			printf("Limiter is %s\n", redir->limiter);
		redirs = redirs->next;
	}
}
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
		else
			continue;
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
			print_exec_step(exec_steps);
			exec_steps = exec_steps->next;
		}
		ft_lstclear(&tokens, free_token);
		ft_lstclear(&exec_steps_start, free_exec_step);
		rl_on_new_line();
		free(line);
	}
	clear_history();
}

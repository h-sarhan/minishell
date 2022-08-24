/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/24 14:55:46 by hsarhan          ###   ########.fr       */
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
		printf("===================SUB EXPR START===================\n\n");
		exec_steps = step->subexpr_steps;
		// printf("\n");
		while (exec_steps != NULL)
		{
			print_exec_step(exec_steps);
			exec_steps = exec_steps->next;
		}
		if (step->pipe_next == true)
			printf("Pipe subexpr  into next command\n");
		if (step->and_next == true)
			printf("AND  subexpr into next command\n");
		if (step->or_next == true)
			printf("OR   subexpr into next command\n");
		printf("===================SUB EXPR END===================\n\n");
		// printf("\n");
		return ;
	}
	// printf("\n");
	printf("===================EXPR START===================\n");
	while (args != NULL)
	{
		printf("Arg #%lu == %s\n", i + 1, (char *)args->content);
		args = args->next;
		i++;
	}
	if (step->pipe_next == true)
		printf("Pipe expr  into next command\n");
	if (step->and_next == true)
		printf("AND  expr into next command\n");
	if (step->or_next == true)
		printf("OR   expr into next command\n");
	printf("\n");
	
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
	printf("===================EXPR END===================\n\n");
}

// ? I dont know what rl_on_new_line() does
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
		else
		{
			ft_free(&line);
			continue;
		}
		tokens = tokenize_line(line, &success);
		print_tokens(tokens);
		if (success == false)
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
		ft_free(&line);
	}
	clear_history();
}

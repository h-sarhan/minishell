/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/19 09:28:23 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	parse_token_list(t_list *tokens)
{
	t_token	*token;

	while (tokens != NULL)
	{
		token = tokens->content;
		if (token->type == QUOTED_STRING)
		{
			printf("Single quoted string: \'%s\'\n", token->substr);
		}
		if (token->type == DOUBLE_QUOTED_STRING)
		{
			printf("Double quoted string: \"%s\"\n", token->substr);
		}
		if (token->type == ENV_VAR)
		{
			printf("Environment Variable: %s\n", token->substr);
		}
		if (token->type == INPUT_REDIR)
		{
			printf("Input redirection: %s\n", token->substr);
		}
		if (token->type == OUTPUT_REDIR)
		{
			printf("Output redirection: %s\n", token->substr);
		}
		if (token->type == APPEND_REDIR)
		{
			printf("Append redirection: %s\n", token->substr);
		}
		if (token->type == PIPE)
		{
			printf("Pipe: %s\n", token->substr);
		}
		if (token->type == LAST_EXIT)
		{
			printf("Last exit: %s\n", token->substr);
		}
		if (token->type == HEREDOC)
		{
			printf("Heredoc: %s\n", token->substr);
		}
		if (token->type == NORMAL)
		{
			printf("Normal token: %s\n", token->substr);
		}
		if (token->type == AND)
		{
			printf("AND token: %s\n", token->substr);
		}
		if (token->type == OR)
		{
			printf("OR token: %s\n", token->substr);
		}
		if (token->type == SUB_EXPR)
		{
			printf("Sub expression: (%s)\n", token->substr);
			printf("Sub expression tokens START\n");
			print_tokens(token->sub_tokens);
			printf("Sub expression tokens END\n");
		}
		if (token->type == WILDCARD)
		{
			printf("Wildcard token: %s\n", token->substr);
		}
		tokens = tokens->next;
	}
}

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
			parse_token_list(tokens);
			// print_tokens(tokens);
		}
		ft_lstclear(&tokens, free_token);
		rl_on_new_line(); // I dont know what this does
		free(line);
	}
	clear_history();
}


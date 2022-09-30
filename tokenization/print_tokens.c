/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 16:00:18 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/30 08:36:56 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// // ? This doesnt need to be norminetted
// void	print_tokens_detailed(t_list *tokens)
// {
// 	t_token	*token;

// 	while (tokens != NULL)
// 	{
// 		token = tokens->content;
// 		if (token->type == QUOTED_STRING)
// 		{
// 			printf("Single quoted string: %s\nstart=(%zu)\nend=(%zu)\n",
//  token->substr, token->start,
// 					token->end);
// 		}
// 		if (token->type == DOUBLE_QUOTED_STRING)
// 		{
// 			printf("Double quoted string: %s\nstart=(%zu)\nend=(%zu)\n",
//  token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == ENV_VAR)
// 		{
// 			printf("Environment Variable: %s\nstart=(%zu)\nend=(%zu)\n",
//  token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == INPUT_REDIR)
// 		{
// 			printf("Input redirection: %s\nstart=(%zu)\nend=(%zu)\n",
//  token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == OUTPUT_REDIR)
// 		{
// 			printf("Output redirection: %s\nstart=(%zu)\nend=(%zu)\n",
//  token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == APPEND)
// 		{
// 			printf("Append redirection: %s\nstart=(%zu)\nend=(%zu)\n", 
// token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == PIPE)
// 		{
// 			printf("Pipe: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == LAST_EXIT)
// 		{
// 			printf("Last exit: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == HEREDOC)
// 		{
// 			printf("Heredoc: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == NORMAL)
// 		{
// 			printf("Normal: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == AND)
// 		{
// 			printf("AND token: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == OR)
// 		{
// 			printf("OR token: %s\nstart=(%zu)\nend=(%zu)\n", token->substr,
// 					token->start, token->end);
// 		}
// 		if (token->type == SUB_EXPR)
// 		{
// 			printf("Sub expression: %s\nstart=(%zu)\nend=(%zu)\n"
// , token->substr,
// 					token->start, token->end);
// 			printf("Sub expression START\n");
// 			print_tokens_detailed(token->sub_tokens);
// 			printf("Sub expression END\n");
// 		}
// 		if (token->type == WILDCARD)
// 		{
// 			printf("Wildcard token: %s\nstart=(%zu)\nend=(%zu)\n",
//  token->substr,
// 					token->start, token->end);
// 		}
// 		tokens = tokens->next;
// 	}
// }

// // ? This doesnt need to be norminetted
// void	print_tokens(t_list *tokens)
// {
// 	t_token	*token;

// 	while (tokens != NULL)
// 	{
// 		token = tokens->content;
// 		if (token->type == QUOTED_STRING)
// 			printf("Single quoted string: %s\n", token->substr);
// 		if (token->type == DOUBLE_QUOTED_STRING)
// 			printf("Double quoted string: %s\n", token->substr);
// 		if (token->type == ENV_VAR)
// 			printf("Environment Variable: %s\n", token->substr);
// 		if (token->type == INPUT_REDIR)
// 			printf("Input redirection: %s\n", token->substr);
// 		if (token->type == OUTPUT_REDIR)
// 			printf("Output redirection: %s\n", token->substr);
// 		if (token->type == APPEND)
// 			printf("Append redirection: %s\n", token->substr);
// 		if (token->type == PIPE)
// 			printf("Pipe: %s\n", token->substr);
// 		if (token->type == LAST_EXIT)
// 			printf("Last exit: %s\n", token->substr);
// 		if (token->type == HEREDOC)
// 			printf("Heredoc: %s\n", token->substr);
// 		if (token->type == NORMAL)
// 			printf("Normal token: %s\n", token->substr);
// 		if (token->type == AND)
// 			printf("AND token: %s\n", token->substr);
// 		if (token->type == OR)
// 			printf("OR token: %s\n", token->substr);
// 		if (token->type == SUB_EXPR)
// 		{
// 			printf("Sub expression: %s\n", token->substr);
// 			printf("Sub expression tokens START\n");
// 			print_tokens(token->sub_tokens);
// 			printf("Sub expression tokens END\n");
// 		}
// 		if (token->type == WILDCARD)
// 			printf("Wildcard token: %s\n", token->substr);
// 		tokens = tokens->next;
// 	}
// }
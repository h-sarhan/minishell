/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/05 16:38:06 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	print_exec_step(t_list *exec_steps)
{
	t_exec_step	*step = exec_steps->content;
	t_redir	*redir;
	size_t	i = 0;
	char	**args;
	t_list	*in_redirs = NULL;
	t_list	*out_redirs = NULL;
	
	if (step->cmd != NULL)
	{
		args = step->cmd->arg_arr;
		in_redirs = step->cmd->in_redirs;
		out_redirs = step->cmd->out_redirs;
	}
	if (step->subexpr_steps != NULL)
	{
		printf("===================SUB EXPR START===================\n\n");
		exec_steps = step->subexpr_steps;
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
		return ;
	}
	printf("===================EXPR START===================\n");
	while (args[i] != NULL)
	{
		printf("Arg #%lu == %s\n", i + 1, args[i]);
		i++;
	}
	if (step->pipe_next == true)
		printf("Pipe expr  into next command\n");
	if (step->and_next == true)
		printf("AND  expr into next command\n");
	if (step->or_next == true)
		printf("OR   expr into next command\n");
	printf("\n");
	
	while (in_redirs != NULL)
	{
		redir = in_redirs->content;
		if (redir->type == INPUT_REDIR)
			printf("Input redirection from %s\n", redir->file);
		if (redir->type == HEREDOC)
			printf("Heredoc redir. Limiter is %s\n", redir->limiter);
		in_redirs = in_redirs->next;
	}
	while (out_redirs != NULL)
	{
		redir = out_redirs->content;
		if (redir->type == OUTPUT_REDIR)
			printf("Output redirection to %s\n", redir->file);
		if (redir->type == APPEND)
			printf("Append redirection to %s\n", redir->file);
		out_redirs = out_redirs->next;
	}
	printf("===================EXPR END===================\n\n");
}

// ? I dont know what rl_on_new_line() does
int	main(int argc, char **argv, char **env)
{
	char	*line;
	bool	success;
	t_shell	shell;
	(void)argc;
	(void)argv;
	
	success = true;
	shell.env = copy_str_arr(env);
	while (1)
	{
		line = readline("GIGASHELL$ ");
		if (line == NULL)
		{
			printf("\n");
			free_split_array(shell.env);
			return (EXIT_SUCCESS);
		}
		if (line[0] != '\0')
			add_history(line);
		else
		{
			ft_free(&line);
			continue;
		}
		shell.tokens = tokenize_line(&shell, line, &success);
		print_tokens(shell.tokens);
		if (success == false)
			continue;
		shell.steps = parse_tokens(shell.tokens, &success);
		t_list *exec_steps_start = shell.steps;
		if (success == false)
		{
			write_to_stderr("Parse error\n");
			ft_lstclear(&shell.tokens, free_token);
			ft_lstclear(&exec_steps_start, free_exec_step);
			rl_on_new_line();
			free(line);
			continue;
		}
		while (shell.steps != NULL)
		{
			print_exec_step(shell.steps);
			shell.steps = shell.steps->next;
		}
		shell.steps = exec_steps_start;
		if (exec_steps_start != NULL)
		{
			t_exec_step	*step;
			step = exec_steps_start->content;
			// if (step->cmd->arg_arr[0] != NULL
			// 	&& ft_strncmp(step->cmd->arg_arr[0], "env", ft_strlen("env")) == 0)
			// {
			// 	ft_env(&shell, step);
			// }
			// if (step->cmd->arg_arr[0] != NULL
			// 	&& ft_strncmp(step->cmd->arg_arr[0], "export", ft_strlen("export")) == 0)
			// {
			// 	ft_export(&shell, step);
			// }
			// printf("%s\n", (char *)((t_exec_step *)shell.steps->content)->cmd->arg_arr[0]);
			if (!is_builtin(step, &shell, line))
				exec_cmd(&shell);
			
		}
		ft_lstclear(&shell.tokens, free_token);
		ft_lstclear(&exec_steps_start, free_exec_step);
		rl_on_new_line();
		ft_free(&line);
	}
	free_split_array(shell.env);
	clear_history();
}

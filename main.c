/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/15 12:22:18 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	print_exec_step(t_list *exec_steps)
{
	t_exec_step	*step = exec_steps->content;
	t_redir	*redir;
	size_t	i = 0;
	char	**args;
	t_list	*redirs = NULL;
	
	if (step->cmd != NULL)
	{
		args = step->cmd->arg_arr;
		redirs = step->cmd->redirs;
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
	
	while (redirs != NULL)
	{
		redir = redirs->content;
		if (redir->type == OUTPUT_REDIR)
			printf("Output redirection to %s\n", redir->file);
		if (redir->type == APPEND)
			printf("Append redirection to %s\n", redir->file);
		if (redir->type == INPUT_REDIR)
			printf("Input redirection from %s\n", redir->file);
		if (redir->type == HEREDOC)
			printf("Heredoc redir. Limiter is %s\n", redir->limiter);
		redirs = redirs->next;
	}
}

// void	reciever(int sig, siginfo_t *siginfo, void *context)
// {
// 	if (sig == SIGINT)
// }


bool	g_interactive;
int 	g_dupstdin;

// bool	g_interactive;
// int		stdin_dup;

void	handler(int	sig)
{
	if (sig == SIGINT)
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		if (g_interactive == true)
			rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		if (g_interactive == false)
		{

			write(2, "QUIT\n", ft_strlen("QUIT\n"));
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else
		{
			// rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		// return ;
	}
}

void test(int sig)
{
	if (sig == SIGINT)
	{
		ft_close(&g_dupstdin);
		printf("\n");
	}
}

// ? I dont know what rl_on_new_line() does
int	main(int argc, char **argv, char **env)
{
	char	*line;
	bool	success;
	t_shell	shell;
	// sigaction struct sa;
	
	(void)argc;
	(void)argv;
	success = true;
	shell.env = copy_str_arr(env);
	// sa.sa_sigaction = reciever;
	// sigemptyset(&sa.sa_mask);
	// sa.sa_flags = SA_SIGINFO;
	shell.last_exit_code = 0;
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	// signal(SIGQUIT, test);
	// sigaction(SIGKILL, &sa, NULL);
	while (1)
	{
		g_dupstdin = dup(0);
		g_interactive = true;
		line = readline("GIGASHELL$ ");
		g_interactive = false;
		if (line == NULL)
		{
			printf("\n");
			free_split_array(shell.env);
			ft_close(&g_dupstdin);
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
		// print_tokens(shell.tokens);
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
			ft_close(&g_dupstdin);
			free(line);
			continue;
		}
		signal(SIGINT, test);
		signal(SIGQUIT, test);
		while (shell.steps != NULL)
		{
			// print_exec_step(shell.steps);
			run_here_docs(shell.steps->content);
			shell.steps = shell.steps->next;
		}
		if (g_dupstdin == -1)
		{
			ft_lstclear(&shell.tokens, free_token);
			ft_lstclear(&exec_steps_start, free_exec_step);
			rl_on_new_line();
			ft_free(&line);
			ft_close(&g_dupstdin);
			continue;
		}
		signal(SIGINT, handler);
		signal(SIGQUIT, handler);
		shell.steps = exec_steps_start;
		if (exec_steps_start != NULL)
		{
			exec_cmd(&shell);
		}
		ft_lstclear(&shell.tokens, free_token);
		ft_lstclear(&exec_steps_start, free_exec_step);
		rl_on_new_line();
		ft_free(&line);
		ft_close(&g_dupstdin);
	}
	ft_close(&g_dupstdin);
	free_split_array(shell.env);
	clear_history();
}

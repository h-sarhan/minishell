/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/25 18:56:17 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int 	g_dupstdin;

static bool	check_subexpr(t_shell *shell, t_exec_step *step)
{
	t_list		*tokens;
	bool		success;
	t_list		*steps;

	tokens = tokenize_line(shell, step->subexpr_line, &success);
	if (success == false)
	{
		ft_lstclear(&tokens, free_token);
		return (false);
	}
	steps = parse_tokens(tokens, &success);
	ft_lstclear(&tokens, free_token);
	if (success == false || check_subexprs(shell, steps) == false)
	{
		ft_lstclear(&steps, free_exec_step);
		return (false);
	}
	ft_lstclear(&steps, free_exec_step);
	return (true);
}


bool	check_subexprs(t_shell *shell, t_list *shell_steps)
{
	t_exec_step *step;

	while (shell_steps != NULL)
	{
		step = shell_steps->content;
		if (step->subexpr_line != NULL)
		{
			if (check_subexpr(shell, step) == false)
			{
				return (false);
			}
		}
		shell_steps = shell_steps->next;
	}
	return (true);
}

// ? I dont know what rl_on_new_line() does
int	main(int argc, char **argv, char **env)
{
	char	*line;
	bool	success;
	t_shell	shell;

	// rl_outstream = stderr;
	(void)argc;
	(void)argv;
	success = true;
	shell.env = copy_str_arr(env);
	shell.declared_env = NULL;
	shell.steps_to_free = NULL;
	unset_var(&shell, "OLDPWD");
	update_declared_env(&shell, "OLDPWD");
	char *shell_lvl_env = get_env(&shell, "SHLVL");
	shell.fd = ft_calloc(2, sizeof(int));
	if (shell_lvl_env != NULL)
	{
		char	*shell_lvl_str = strjoin_free("SHLVL=", ft_itoa(ft_atoi(shell_lvl_env) + 1), 2);
		update_env(&shell, shell_lvl_str);
		ft_free(&shell_lvl_str);
		ft_free(&shell_lvl_env);
	}
	shell.last_exit_code = 0;
	while (1)
	{
		g_dupstdin = dup(0);
		signal(SIGINT, sigint_interactive);
		// signal(SIGQUIT, sigquit_interactive);
		signal(SIGQUIT, SIG_IGN);
		line = readline("\001\033[1;34m\002GIGASHELL$ \001\033[0m\002");
		if (g_dupstdin == -1)
		{
			g_dupstdin = dup(0);
			shell.last_exit_code = 1;
		}
		// line = readline("GIGASHELL$ ");
		shell.line = line;
		signal(SIGQUIT, sigquit_command);
		signal(SIGINT, sigint_command);
		// signal(SIGQUIT, SIG_IGN);
		// signal(SIGINT, SIG_IGN);
		if (line == NULL)
		{
			// write(2, "\n", 1);
			// printf("exit\n");
			free_split_array(shell.env);
			free_split_array(shell.declared_env);
			ft_close(&g_dupstdin);
			ft_free(&shell.fd);
			return (shell.last_exit_code);
		}
		if (line[0] != '\0')
			add_history(line);
		else
		{
			ft_close(&g_dupstdin);
			ft_free(&line);
			continue;
		}
		shell.tokens = tokenize_line(&shell, line, &success);
		// print_tokens(shell.tokens);
		if (success == false)
		{
			shell.last_exit_code = 258;
			ft_lstclear(&shell.tokens, free_token);
			ft_close(&g_dupstdin);
			ft_free(&line);
			continue;
		}
		shell.steps = parse_tokens(shell.tokens, &success);
		ft_lstadd_back(&shell.steps_to_free, ft_lstnew(shell.steps));
		t_list *exec_steps_start = shell.steps;
		if (success == false)
		{
			shell.last_exit_code = 258;
			write_to_stderr("Parse error\n");
			ft_lstclear(&shell.tokens, free_token);
			// ft_lstclear(&exec_steps_start, free_exec_step);
			free_steps(&shell.steps_to_free);
			rl_on_new_line();
			ft_close(&g_dupstdin);
			ft_free(&line);
			continue;
		}
		// shell.steps = exec_steps_start;
		if (check_subexprs(&shell, shell.steps) == false)
		{
			shell.last_exit_code = 258;
			write_to_stderr("Parse error\n");
			ft_lstclear(&shell.tokens, free_token);
			// ft_lstclear(&exec_steps_start, free_exec_step);
			free_steps(&shell.steps_to_free);
			rl_on_new_line();
			ft_close(&g_dupstdin);
			free(line);
			continue;
		}
		signal(SIGINT, hd_sig_handler);
		signal(SIGQUIT, hd_sig_handler);
		while (shell.steps != NULL)
		{
			// print_exec_step(shell.steps);
			run_here_docs(&shell, shell.steps->content);
			shell.steps = shell.steps->next;
		}
		signal(SIGINT, sigint_command);
		signal(SIGQUIT, sigquit_command);
		if (g_dupstdin == -1)
		{
			shell.last_exit_code = 1;
			ft_lstclear(&shell.tokens, free_token);
			free_steps(&shell.steps_to_free);
			rl_on_new_line();
			ft_free(&line);
			ft_close(&g_dupstdin);
			continue;
		}
		shell.steps = exec_steps_start;
		if (exec_steps_start != NULL)
		{
			shell.line = line;
			exec_cmd(&shell, shell.steps, 0, shell.line);
		}
		// if (g_dupstdin == SIGQUIT_FLAG)
		// {
		// 	shell.last_exit_code = 131;
		// }
		// if (g_dupstdin == SIGINT_FLAG)
		// {
		// 	shell.last_exit_code = 130;
		// }
		ft_lstclear(&shell.tokens, free_token);
		// ft_lstclear(&shell.steps, free_exec_step);
		free_steps(&shell.steps_to_free);
		rl_on_new_line();
		ft_free(&line);
		ft_close(&g_dupstdin);
	}
	ft_close(&g_dupstdin);
	free_split_array(shell.env);
	free_split_array(shell.declared_env);
	ft_free(&shell.fd);
	clear_history();
}

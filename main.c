/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:43:26 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/30 07:54:22 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_dupstdin;

void	init_shell(t_shell *shell, char **env)
{
	char	*shelvl_env;
	char	*shlvl_str;

	shell->env = copy_str_arr(env);
	shell->declared_env = NULL;
	shell->steps_to_free = NULL;
	unset_var(shell, "OLDPWD");
	update_declared_env(shell, "OLDPWD");
	shelvl_env = get_env(shell, "SHLVL");
	shell->fd = ft_calloc(2, sizeof(int));
	if (shell->fd == NULL)
	{
		free_split_array(shell->env);
		ft_free(&shelvl_env);
		exit(EXIT_FAILURE);
	}
	if (shelvl_env != NULL)
	{
		shlvl_str = strjoin_free("SHLVL=", ft_itoa(ft_atoi(shelvl_env) + 1), 2);
		update_env(shell, shlvl_str);
		ft_free(&shlvl_str);
		ft_free(&shelvl_env);
	}
	shell->last_exit_code = 0;
}

bool	add_to_history(const char *line)
{
	if (line[0] != '\0')
	{
		add_history(line);
		return (true);
	}
	else
	{
		ft_close(&g_dupstdin);
		ft_free(&line);
		return (false);
	}
}

bool	handle_tokenization_error(t_shell *shell, bool success, char *line)
{
	if (success == false)
	{
		shell->last_exit_code = 258;
		ft_lstclear(&shell->tokens, free_token);
		ft_close(&g_dupstdin);
		ft_free(&line);
		return (false);
	}
	return (true);
}

bool	handle_parsing_error(t_shell *shell, bool success, char *line)
{
	if (success == false || check_subexprs(shell, shell->steps) == false)
	{
		shell->last_exit_code = 258;
		ft_stderr("Parse error\n");
		ft_lstclear(&shell->tokens, free_token);
		free_steps(&shell->steps_to_free);
		rl_on_new_line();
		ft_close(&g_dupstdin);
		ft_free(&line);
		return (false);
	}
	return (true);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	bool	success;
	t_shell	shell;

	(void) argc;
	(void) argv;
	init_shell(&shell, env);
	while (1)
	{
		line = minishell_readline(&shell);
		if (add_to_history(line) == false)
			continue ;
		success = true;
		shell.tokens = tokenize_line(&shell, line, &success);
		if (handle_tokenization_error(&shell, success, line) == false)
			continue ;
		shell.steps = parse_tokens(shell.tokens, &success);
		ft_lstadd_back(&shell.steps_to_free, ft_lstnew(shell.steps));
		if (handle_parsing_error(&shell, success, line) == false)
			continue ;
		minishell_run_heredocs(&shell);
		if (handle_heredoc_ctrl_c(&shell, line) == false)
			continue ;
		minishell_exec_and_cleanup(&shell, line);
	}
}

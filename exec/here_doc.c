/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 14:52:15 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/25 16:24:00 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
extern int g_interactive;
extern int g_dupstdin;


/**
 * @brief Function reads from standard input and returns the content.
 * But the here doc fails when the limiter is the first line.
 * 
 * @param limiter 
 * @return char* 
 */
char	*read_from_stdin(t_shell *shell, char *limiter)
{
	char	*buff;
	// int		buffer_len;
	// int		i;
	// char	ch;
	char	*line;

	// printf("STARTING HEREDOC\n");
	// buffer_len = 100;
	buff = ft_calloc(1, sizeof(char));
	if (buff == NULL)
		return (NULL);
	limiter = strjoin_free(limiter, "\n", 0);
	// i = 0;
	// ft_stderr("> ");
	while (1)
	{
		// if (i == buffer_len)
		// 	buff = resize(&buff, &buffer_len);
		// if (read(0, &ch, 1) < 1)
		// 	break ;
		// if (ch == '\n')
		// {
		// 	ft_stderr("> ");
		// }
		if (g_dupstdin != -1)
			ft_putstr_fd("> ", 1);
		line = get_next_line(g_dupstdin);
		if (line == NULL)
			break;
		if (ft_strcmp(line, limiter) == 0)
			break;
		while (contains_env_var(line) == true)
		{
			line = expand_double_quote(shell, line);
		}
		// if (g_interactive == false)
		// 	break;
		buff = strjoin_free(buff, line, 3);
		// buff[i] = ch;
		// i++;
	}
	ft_free(&limiter);
	ft_free(&line);
	// to free the static variable
	get_next_line(-1);
	// buff[i] = '\0';
	// if (ft_strnstr(buff, limiter, ft_strlen(buff)) != NULL)
	// 	ft_strnstr(buff, limiter, ft_strlen(buff))[0] = '\0';
	return (buff);
}



void	run_here_docs(t_shell *shell, t_exec_step *step)
{
	t_redir *redir;
	t_list *redir_lst;

	// if (step->subexpr_steps != NULL)
	// {
	// 	// ! Remove this cast
	// 	ft_lstiter(step->subexpr_steps, (void (*)(void *))run_here_docs);
	// 	return;
	// }
	if (step->cmd == NULL)
	{
		return ;
	}
	redir_lst = step->cmd->redirs;
	while (redir_lst)
	{
		redir = redir_lst->content;
		if (redir->type == HEREDOC)
		{
			ft_free(&step->cmd->heredoc_contents);
			step->cmd->heredoc_contents =  read_from_stdin(shell, redir->limiter);
			// printf("PRINTING HEREDOC CONTENTS: |%s|", step->cmd->heredoc_contents);
		}
		redir_lst = redir_lst->next;
	}
}
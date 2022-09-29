/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 14:52:15 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/29 09:53:22 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern int	g_dupstdin;

/**
 * @brief Function reads from standard input and returns the content.
 * But the here doc fails when the limiter is the first line.
 * 
 * @param limiter 
 * @return char* 
 */
static char	*read_from_stdin(t_shell *shell, char *limiter)
{
	char	*buff;
	char	*line;

	buff = ft_calloc(1, sizeof(char));
	if (buff == NULL)
		return (NULL);
	limiter = strjoin_free(limiter, "\n", 0);
	while (1)
	{
		if (g_dupstdin != -1)
			ft_putstr_fd("> ", 1);
		line = get_next_line(g_dupstdin);
		if (line == NULL)
			break ;
		if (ft_strcmp(line, limiter) == 0)
			break ;
		while (contains_env_var(line) == true)
			line = expand_env_var(shell, line);
		buff = strjoin_free(buff, line, 3);
	}
	ft_free(&limiter);
	ft_free(&line);
	get_next_line(-1);
	return (buff);
}

/**
 * @brief Runs the herdoc, reads the herdoc content 
 * from standard input and displays it.
 * 
 * @param shell 
 * @param step 
 */
void	run_here_docs(t_shell *shell, t_exec_step *step)
{
	t_redir	*redir;
	t_list	*redir_lst;

	if (step->cmd == NULL)
		return ;
	redir_lst = step->cmd->redirs;
	while (redir_lst)
	{
		redir = redir_lst->content;
		if (redir->type == HEREDOC)
		{
			ft_free(&step->cmd->heredoc_contents);
			step->cmd->heredoc_contents = read_from_stdin(shell,
					redir->limiter);
		}
		redir_lst = redir_lst->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 14:52:15 by mkhan             #+#    #+#             */
/*   Updated: 2022/09/12 16:05:53 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*resize(char **arr, int *old_len)
{
	int		i;
	char	*new_arr;

	i = 0;
	new_arr = ft_calloc(*old_len * 2 + 1, sizeof(char));
	// if (new_arr == NULL)
	// {
	// 	ft_free(arr);
	// 	ft_free(&limiter);
	// 	exit(1);
	// }
	while (i < *old_len)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	free(*arr);
	*old_len *= 2;
	return (new_arr);
}

/**
 * @brief Function reads from standard input and returns the content.
 * But the here doc fails when the limiter is the first line.
 * 
 * @param limiter 
 * @return char* 
 */
char	*read_from_stdin(char *limiter)
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
		line = readline("> ");
		if (ft_strcmp(line, limiter) == 0)
			break;
		line = strjoin_free(line, "\n", 1);
		buff = strjoin_free(buff, line, 3);
		// buff[i] = ch;
		// i++;
	}
	// buff[i] = '\0';
	// if (ft_strnstr(buff, limiter, ft_strlen(buff)) != NULL)
	// 	ft_strnstr(buff, limiter, ft_strlen(buff))[0] = '\0';
	return (buff);
}



void	run_here_docs(t_exec_step *step)
{
	t_redir *redir;
	t_list *redir_lst;

	redir_lst = step->cmd->redirs;
	while (redir_lst)
	{
		redir = redir_lst->content;
		if (redir->type == HEREDOC)
		{
			ft_free(&step->cmd->heredoc_contents);
			step->cmd->heredoc_contents =  read_from_stdin(redir->limiter);
			// printf("PRINTING HEREDOC CONTENTS: |%s|", step->cmd->heredoc_contents);
		}
		redir_lst = redir_lst->next;
	}
}
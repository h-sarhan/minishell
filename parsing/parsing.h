/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 11:55:31 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 17:55:30 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

typedef struct s_exec_step	t_exec_step;
typedef struct s_cmd		t_cmd;
typedef struct s_redir		t_redir;


struct	s_cmd
{
	t_list	*args;
	int		pid;
	int		exit_code;
	t_list	*redirs;
};

struct	s_redir
{
	t_token_type	type;
	char			*file;
};

struct	s_exec_step
{
	t_cmd	*cmd;
	bool	pipe_next;
};
t_list	*parse_tokens(t_list *tokens, bool *success);

#endif
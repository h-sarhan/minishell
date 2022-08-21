/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 11:55:31 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 13:13:20 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

typedef struct s_exec_step	t_exec_step;
typedef struct s_cmd		t_cmd;


struct	s_cmd
{
	char	**args;
	int		pid;
	int		exit_code;
	bool	pipe;
	bool	out_redir;
	char	*out_redir_file;
	bool	in_redir;
	char	*in_redir_file;
	int		exit_code;
};

struct	s_exec_step
{
	t_cmd	*cmd;
};
t_list	*parse_tokens(t_list *tokens);

#endif
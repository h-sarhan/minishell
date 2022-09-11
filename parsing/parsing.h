/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 11:55:31 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/11 13:46:21 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

typedef struct s_exec_step	t_exec_step;
typedef struct s_cmd		t_cmd;
typedef struct s_redir		t_redir;

struct	s_redir
{
	t_token_type	type;
	char			*file;
	char			*limiter;
};

struct	s_cmd
{
	t_list	*args;
	char	**arg_arr;
	int		pid;
	t_list	*redirs;
};

struct	s_exec_step
{
	t_cmd	*cmd;
	t_list	*subexpr_steps;
	int		exit_code;
	bool	pipe_next;
	bool	and_next;
	bool	or_next;
};

t_list	*parse_tokens(t_list *tokens, bool *success);
void	free_exec_step(void *exec_step_ptr);
void	free_redir(void *redir_ptr);
void	print_exec_step(t_list *exec_steps);
void	list_to_str_arr(void *step_ptr);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 11:55:31 by hsarhan           #+#    #+#             */
/*   Updated: 2023/03/21 19:10:43 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "tokenization.h"

typedef struct s_exec_step	t_exec_step;
typedef struct s_cmd		t_cmd;
typedef struct s_redir		t_redir;
typedef struct s_shell		t_shell;
typedef struct s_list		t_list;

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
	char	*subexpr_line;
	int		exit_code;
	bool	pipe_next;
	bool	and_next;
	bool	or_next;
};

t_list		*parse_tokens(t_list *tokens, bool *success);
void		free_exec_step(void *exec_step_ptr);
void		free_redir(void *redir_ptr);
void		print_exec_step(t_list *exec_steps);
void		list_to_str_arr(void *step_ptr);
bool		is_terminator(const t_token *token);
bool		is_redirection(const t_token *token);
bool		check_for_errors(t_list *tokens);
bool		parsing_error(t_redir *redir, t_exec_step *step);
bool		check_next_subexpr_token(t_list *tokens, t_token **token,
				t_exec_step *step, bool *success);
bool		check_next_token(t_list *cmd_end, t_token **token,
				t_exec_step *step, t_list *tokens);
t_list		*parse_step(t_list **tokens, t_token **token, t_list **steps,
				bool *success);

#endif
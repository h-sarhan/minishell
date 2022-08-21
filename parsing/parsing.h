/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 11:55:31 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/21 12:04:17 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

typedef struct s_exec_step	t_exec_step;
typedef struct s_cmd		t_cmd;
// typedef enum e_step_type	t_step_type;

// // Probably dont need this enum
// enum	e_step_type
// {
// 	CMD,
// 	PIPE,
// 	INPUT_REDIR,
// 	OUTPUT_REDIR,
// 	APPEND_REDIR,
// 	// ? SUB_EXPR,
// 	AND,
// 	OR,
// 	HEREDOC,
// };

struct	s_cmd
{
	char	**args;
	int		pid;
	int		exit_code;
};

struct	s_exec_step
{
	t_cmd	*cmd;
};

#endif
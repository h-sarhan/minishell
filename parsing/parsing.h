/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:44:00 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/17 18:39:08 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>

typedef enum e_token_type	t_token_type;
typedef struct s_token		t_token;

enum	e_token_type
{
	QUOTED_STRING,
	DOUBLE_QUOTED_STRING,
	NORMAL_TOKEN,
	ENV_VAR,
	INPUT_REDIR,
	OUTPUT_REDIR,
	APPEND_REDIR,
	HEREDOC,
	HEREDOC_DELIM,
	PIPE,
	LAST_EXIT,
};

struct s_token
{
	t_token_type	type;
	bool			expanded;
	size_t			start;
	size_t			end;
};

t_list	*parse_line(const char *line, bool *success);
void	write_to_stderr(const char *msg);
void	print_tokens(const char *line, t_list *tokens);
#endif
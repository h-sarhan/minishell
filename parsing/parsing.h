/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:44:00 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/17 21:26:51 by hsarhan          ###   ########.fr       */
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
	size_t			start;
	size_t			end;
	char			*substr;
};

t_list	*parse_line(const char *line, bool *success);
void	write_to_stderr(const char *msg);
void	print_tokens(t_list *tokens);
char	*strjoin_free(char *s1, char *s2, int f);
char	*substr_free(char *s, unsigned int start, size_t len);
char	*expand_env_var(char *str);
void	ft_free(void *memory);
#endif
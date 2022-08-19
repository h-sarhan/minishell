/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:44:00 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/19 13:45:53 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <dirent.h>

typedef enum e_token_type	t_token_type;
typedef struct s_token		t_token;

enum	e_token_type
{
	QUOTED_STRING,
	DOUBLE_QUOTED_STRING,
	NORMAL,
	ENV_VAR,
	INPUT_REDIR,
	OUTPUT_REDIR,
	APPEND_REDIR,
	HEREDOC,
	PIPE,
	LAST_EXIT,
	SUB_EXPR,
	AND,
	OR,
	WILDCARD
};

struct s_token
{
	t_token_type	type;
	size_t			start;
	size_t			end;
	char			*substr;
	t_list			*sub_tokens;
};

t_list	*tokenize_line(const char *line, bool *success);
void	write_to_stderr(const char *msg);
void	print_tokens_detailed(t_list *tokens);
char	*strjoin_free(char *s1, char *s2, int f);
char	*substr_free(char *s, unsigned int start, size_t len);
bool	contains_env_var(const char *str);
char	*expand_double_quote(char *str);
void	ft_free(void *memory);
t_list	*tokenize_operator(const char *line, size_t *idx, const t_token_type type);
t_list	*tokenize_normal(const char *line, size_t *idx);
t_list	*tokenize_subexpr(const char *line, size_t *idx);
char	*get_dir_contents(void);
char	*expand_wildcard(char *token);
char**	split_wildcard(char *wildcard_token);
void	free_token(void *mem);
void	free_split_array(char **split);

#endif
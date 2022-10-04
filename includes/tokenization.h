/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:44:00 by hsarhan           #+#    #+#             */
/*   Updated: 2022/10/04 09:24:11 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_H
# define TOKENIZATION_H

# include "libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <dirent.h>

# define TRUE 1
# define FALSE 0
# define CONTINUE 2 

typedef enum e_token_type	t_token_type;
typedef struct s_token		t_token;
typedef struct s_shell		t_shell;
typedef struct s_wildcard	t_wildcard;

enum	e_token_type
{
	QUOTED_STRING,
	DOUBLE_QUOTED_STRING,
	NORMAL,
	INPUT_REDIR,
	OUTPUT_REDIR,
	APPEND,
	HEREDOC,
	PIPE,
	AND,
	OR,
	SUB_EXPR,
	LAST_EXIT,
	ENV_VAR,
	WILDCARD,
	DUMMY
};

struct s_token
{
	t_token_type	type;
	size_t			start;
	size_t			end;
	char			*substr;
	t_list			*sub_tokens;
	bool			expanded;
};

struct s_wildcard
{
	char	*str;
	bool	is_wildcard;
};

char		*eat_dollars(const char *str);
t_list		*tokenize_line(const t_shell *shell, const char *line,
				bool *success);
t_list		*tokenize_env_var_str(const char *line, bool *success);
t_list		*tokenize_env_variable(const t_shell *shell, const char *line,
				size_t *idx);
void		print_tokens_detailed(t_list *tokens);
char		*strjoin_free(char *s1, char *s2, int f);
char		*substr_free(char *s, unsigned int start, size_t len);
bool		contains_env_var(const char *str);
char		*expand_env_var(const t_shell *shell, char *str);
void		ft_free(void *memory);
t_list		*tokenize_operator(const char *line, size_t *idx,
				const t_token_type type);
t_list		*tokenize_subexpr(const t_shell *shell, const char *line,
				size_t *idx);
char		*expand_wildcard(char *token);
t_wildcard	**split_wildcard(const char *wc);
void		free_token(void *mem);
void		free_split_array(char **split);
t_list		*tokenize_single_quote(const t_shell *shell, const char *line,
				size_t *idx);
t_list		*tokenize_double_quote(const t_shell *shell, const char *line,
				size_t *idx, const bool expand_var);
void		print_tokens_detailed(t_list *tokens);
void		print_tokens(t_list *tokens);
char		*eat_quotes(const char *str);
t_list		*tokenize_normal(const t_shell *shell, const char *line,
				size_t *idx, bool expand_var);
void		set_in_quotes(const char *str, const size_t i, bool *in_single,
				bool *in_double);
bool		match_str_on_wildcard(const char *str, t_wildcard **wc_segs);
bool		is_operator(const char *line, const size_t i);
t_list		*tokenize_operator_token(const char *line, size_t *i);
void		*token_error(const char *msg, t_list **tokens, bool *success);
bool		last_token_was_heredoc(t_list *tokens);
bool		tokenize_wildcard(const t_shell *shell, t_list **el,
				t_list **tokens, bool *success);
void		set_quotes(const char ch, char *quote, bool *in_quote);
bool		check_for_token_errors(const char *line, bool *success);
void		tokenize_env_cleanup(t_list **el,
				t_list **tokens, bool *success);
bool		first_token_group(const t_shell *shell, const char *line,
				size_t *i, t_list **tokens);
bool		second_token_group(const t_shell *shell, const char *line,
				size_t *i, t_list **tokens);
bool		tokenize_normal_and_wildcard(const t_shell *shell, const char *line,
				size_t *i, t_list **tokens);
bool		handle_quotes_count(const char *str, size_t *num_words, size_t *i);
bool		handle_quotes_split(const char *wc, size_t *i, size_t *word_count,
				t_wildcard **wc_split);
char		*create_word(char const *str, const size_t word_start,
				const size_t word_end);
t_wildcard	*create_wc(char *str, bool is_wildcard);

#endif
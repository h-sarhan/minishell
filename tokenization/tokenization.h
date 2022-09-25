/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 11:44:00 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/25 15:36:54 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_H
# define TOKENIZATION_H

# include "../libft/libft.h"
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
	WILDCARD
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
char *eat_dollars(const char *str);
t_list	*tokenize_line(const t_shell *shell, const char *line, bool *success);
t_list	*tokenize_env_variable(const t_shell *shell, const char *line, size_t *idx);
void	write_to_stderr(const char *msg);
void	print_tokens_detailed(t_list *tokens);
char	*strjoin_free(char *s1, char *s2, int f);
char	*substr_free(char *s, unsigned int start, size_t len);
bool	contains_env_var(const char *str);
char	*expand_double_quote(const t_shell *shell, char *str);
void	ft_free(void *memory);
t_list	*tokenize_operator(const char *line, size_t *idx,
			const t_token_type type);
t_list	*tokenize_subexpr(const t_shell *shell, const char *line, size_t *idx);
char	*get_dir_contents(void);
char	*expand_wildcard(char *token);
char	**split_wildcard(const char *wildcard_token);
void	free_token(void *mem);
void	free_split_array(char **split);
t_list	*tokenize_single_quote(const t_shell *shell, const char *line, size_t *idx);
t_list	*tokenize_double_quote(const t_shell *shell, const char *line, size_t *idx);
void	print_tokens_detailed(t_list *tokens);
void	print_tokens(t_list *tokens);
char	*join_tokens(t_list *tokens);
char	*eat_quotes(const char *str);
t_list	*tokenize_normal(const t_shell *shell, const char *line, size_t *idx, bool expand_var);
#endif
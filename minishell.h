/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 09:39:31 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/29 11:15:02 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "tokenization/tokenization.h"
# include "parsing/parsing.h"
# include "builtins/builtins.h"
# include "exec/exec.h"
# include "ft_printf/ft_printf.h"
# include "signals/signals.h"

# include <readline/readline.h>
# include <readline/history.h>
# include <sys/errno.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <signal.h>

int						g_dupstdin;

typedef struct s_shell	t_shell;
struct s_shell
{
	t_list	*steps;
	t_list	*tokens;
	t_list	*steps_to_free;
	char	*current_line;
	char	**env;
	char	**declared_env;
	int		last_exit_code;
	char	*line;
	int		*fd;
	t_list	*heredoc_contents;
};

bool	check_subexprs(t_shell *shell, t_list *shell_steps);
void	ft_close(int *fd);
char	**copy_str_arr(char **arr);
void	free_steps(t_list **step_lists);
void	sigint_interactive(int sig);
void	sigint_command(int sig);
void	sigquit_command(int sig);
void	hd_sig_handler(int sig);
char	*get_env(const t_shell *shell, const char *name);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 09:39:31 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/07 16:09:24 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "tokenization/tokenization.h"
# include "parsing/parsing.h"
# include "builtins/builtins.h"
# include "exec/exec.h"
# include "ft_printf/ft_printf.h"

# include <readline/readline.h>
# include <readline/history.h>
#include <sys/errno.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

typedef struct s_shell	t_shell;

struct s_shell
{
	t_list	*steps;
	t_list	*tokens;
	char	**env;
	int		last_exit_code;
};

struct signal
{
	t_list	*pid;
	bool	interactive_mode;
	
};


char	**copy_str_arr(char **arr);
// void	rl_replace_line(const char *text, int clear_undo);

#endif
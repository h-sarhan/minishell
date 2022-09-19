/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 09:39:31 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/19 10:59:43 by hsarhan          ###   ########.fr       */
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
	char	**declared_env;
	int		last_exit_code;
	char	*line;
};

struct signal
{
	t_list	*pid;
	bool	interactive_mode;
	
};

void	ft_close(int *fd);
char	**copy_str_arr(char **arr);

#endif
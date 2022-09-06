/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 09:39:31 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/06 14:48:06 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "tokenization/tokenization.h"
# include "parsing/parsing.h"
# include "builtins/builtins.h"
# include "exec/exec.h"
# include "ft_printf/ft_printf.h"

#include <sys/errno.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct s_shell	t_shell;

struct s_shell
{
	t_list	*steps;
	t_list	*tokens;
	char	**env;
	int		last_exit_code;
};

char	**copy_str_arr(char **arr);

#endif
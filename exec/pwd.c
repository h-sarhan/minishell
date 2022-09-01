/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 21:56:29 by mkhan             #+#    #+#             */
/*   Updated: 2022/08/30 13:45:15 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(t_exec_step *step)
{
	(void)step;
	char	*dir;
	
	dir = getcwd(NULL, 0);
	printf("%s\n", dir);
	ft_free(&dir);
	step->exit_code = 0;
}

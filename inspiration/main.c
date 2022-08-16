/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:01:53 by hsarhan           #+#    #+#             */
/*   Updated: 2022/07/04 13:10:05 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Replicates the behaviour of 
// `< infile cmd1 | cmd2 | cmd3 | ... | cmdN > outfile` in bash
int	main(int argc, char **argv, char **env)
{
	t_list	*command_list;
	t_list	*first;
	int		fds[2];
	int		pipe_fds[2];

	check_args(argc, argv);
	fds[0] = -1;
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
		command_list = handle_here_doc(argv, pipe_fds, fds, env);
	else
	{
		fds[0] = open_file(argv[1], 0);
		fds[1] = open_file(argv[argc - 1], 1);
		command_list = create_command_list(argc, argv, fds, env);
		if (command_list == NULL)
			exit(EXIT_FAILURE);
		ft_pipe(pipe_fds, command_list);
		handle_first_cmd(command_list->content, fds, pipe_fds, command_list);
	}
	if (command_list == NULL)
		exit(EXIT_FAILURE);
	first = command_list;
	command_list = command_list->next;
	command_list = handle_mid_cmds(command_list, pipe_fds, fds, first);
	handle_last_cmd(command_list->content, fds, pipe_fds, first);
}

//                                           
//    ▓▓▓▓▓▓▒▒▓▓▓▓▒▒▒▒                        
//    ▓▓▓▓▒▒▒▒▒▒▒▒░░▒▒         ██╗░░██╗       
//    ▓▓▒▒▒▒▓▓▒▒▓▓░░▒▒         ╚██╗██╔╝       
//      ▓▓▒▒▓▓██▒▒▒▒           ░╚███╔╝░       
//      ▒▒▒▒▒▒▒▒░░▓▓           ░██╔██╗░       
//      ▒▒▒▒▓▓▓▓░░▒▒           ██╔╝╚██╗       
//      ██▓▓▒▒▒▒░░▓▓           ╚═╝░░╚═╝       
//      ▓▓▓▓▓▓▓▓▒▒▓▓                          
//                                            

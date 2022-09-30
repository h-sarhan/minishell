// ! WILL NOT BE SUBMITTING THIS
// void	print_exec_step(t_list *exec_steps)
// {
// 	t_exec_step	*step = exec_steps->content;
// 	t_redir	*redir;
// 	size_t	i = 0;
// 	char	**args;
// 	t_list	*redirs = NULL;

// 	args = step->cmd->arg_arr;
// 	redirs = step->cmd->redirs;
// 	printf("===================EXPR START===================\n");
// 	while (args[i] != NULL)
// 	{
// 		printf("Arg #%lu == %s\n", i + 1, args[i]);
// 		i++;
// 	}
// 	if (step->pipe_next == true)
// 		printf("Pipe expr  into next command\n");
// 	if (step->and_next == true)
// 		printf("AND  expr into next command\n");
// 	if (step->or_next == true)
// 		printf("OR   expr into next command\n");
// 	if (step->subexpr_line != NULL)
// 		printf("subexpr is |%s|\n", step->subexpr_line);
// 	printf("\n");
// 	while (redirs != NULL)
// 	{
// 		redir = redirs->content;
// 		if (redir->type == OUTPUT_REDIR)
// 			printf("Output redirection to %s\n", redir->file);
// 		if (redir->type == APPEND)
// 			printf("Append redirection to %s\n", redir->file);
// 		if (redir->type == INPUT_REDIR)
// 			printf("Input redirection from %s\n", redir->file);
// 		if (redir->type == HEREDOC)
// 			printf("Heredoc redir. Limiter is %s\n", redir->limiter);
// 		redirs = redirs->next;
// 	}
// 	printf("===================EXPR END===================\n");
// }

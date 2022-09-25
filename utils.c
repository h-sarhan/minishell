/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 14:49:04 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/25 18:24:03 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_to_stderr(const char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
}

// This is ft_strjoin but with an additional argument
// to specify which input strings to free
char	*strjoin_free(char *s1, char *s2, int f)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	if (f == 1)
		ft_free(&s1);
	if (f == 2)
		ft_free(&s2);
	if (f == 3)
	{
		ft_free(&s1);
		ft_free(&s2);
	}
	return (joined);
}

char	*substr_free(char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	substr_length;
	size_t	i;

	if (start > ft_strlen(s))
		len = 0;
	else
	{
		substr_length = ft_strlen(&s[start]);
		if (len > substr_length)
			len = substr_length;
	}
	substr = malloc(sizeof(char) * (len + 1));
	if (substr == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = s[i + start];
		i++;
	}
	substr[i] = '\0';
	ft_free(&s);
	return (substr);
}

char	**copy_str_arr(char **arr)
{
	size_t	len;
	size_t	i;
	char	**arr_cpy;

	len = 0;
	while (arr[len] != NULL)
		len++;
	arr_cpy = ft_calloc(len + 1, sizeof(char *));
	i = 0;
	while (i < len)
	{
		arr_cpy[i] = ft_strdup(arr[i]);
		i++;
	}
	arr_cpy[i] = NULL;
	return (arr_cpy);
}

void	free_steps(t_list **step_lists)
{
	t_list	*steps;
	t_list	*temp;

	while ((*step_lists) != NULL)
	{
		steps = (*step_lists)->content;
		temp = (*step_lists)->next;
		ft_lstclear(&steps, free_exec_step);
		ft_free(step_lists);
		(*step_lists) = temp;
	}
	*step_lists = NULL;
}

// ! WILL NOT BE SUBMITTING THIS
void	print_exec_step(t_list *exec_steps)
{
	t_exec_step	*step = exec_steps->content;
	t_redir	*redir;
	size_t	i = 0;
	char	**args;
	t_list	*redirs = NULL;

	args = step->cmd->arg_arr;
	redirs = step->cmd->redirs;
	printf("===================EXPR START===================\n");
	while (args[i] != NULL)
	{
		printf("Arg #%lu == %s\n", i + 1, args[i]);
		i++;
	}
	if (step->pipe_next == true)
		printf("Pipe expr  into next command\n");
	if (step->and_next == true)
		printf("AND  expr into next command\n");
	if (step->or_next == true)
		printf("OR   expr into next command\n");
	printf("\n");
	while (redirs != NULL)
	{
		redir = redirs->content;
		if (redir->type == OUTPUT_REDIR)
			printf("Output redirection to %s\n", redir->file);
		if (redir->type == APPEND)
			printf("Append redirection to %s\n", redir->file);
		if (redir->type == INPUT_REDIR)
			printf("Input redirection from %s\n", redir->file);
		if (redir->type == HEREDOC)
			printf("Heredoc redir. Limiter is %s\n", redir->limiter);
		redirs = redirs->next;
	}
	printf("===================EXPR END===================\n");
}
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/29 21:51:32 by hsarhan           #+#    #+#              #
#    Updated: 2022/08/19 16:26:39 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = parsing/main.c parsing/tokenization.c parsing/parsing_utils.c parsing/expansion.c \
		parsing/tokenization2.c parsing/wildcard.c parsing/split_wildcard.c \
		parsing/print_tokens.c parsing/free_funcs.c parsing/tokenization3.c

OBJS := $(SRCS:%.c=%.o)

NAME = minishell
CC = cc
CFLAGS = -Werror -Wall -Wextra -g -fsanitize=address
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L/usr/local/lib -I/usr/local/include -lreadline $(LIBFT)

$(LIBFT):
	make -C libft


norm:
	norminette | grep Error

clean:
	rm -f $(OBJS)

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re: fclean $(NAME)
	

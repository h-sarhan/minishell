# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/29 21:51:32 by hsarhan           #+#    #+#              #
#    Updated: 2022/08/18 08:01:52 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = parsing/main.c parsing/parsing.c parsing/parsing_utils.c parsing/expansion.c parsing/tokenization.c parsing/wildcard.c

OBJS := $(SRCS:%.c=%.o)

NAME = minishell
CC = cc
CFLAGS = -Werror -Wall -Wextra -fsanitize=address -g
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L/usr/local/lib -I/usr/local/include -lreadline $(LIBFT)

$(LIBFT):
	make -C libft


norm:
	norminette

clean:
	rm -f $(OBJS)

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

re: fclean $(NAME)
	

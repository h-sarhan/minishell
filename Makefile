# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/29 21:51:32 by hsarhan           #+#    #+#              #
#    Updated: 2022/08/17 11:58:25 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = parsing/main.c

OBJS := $(SRCS:%.c=%.o)

NAME = minishell
CC = cc
CFLAGS = -Werror -Wall -Wextra
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline

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
	

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/29 21:51:32 by hsarhan           #+#    #+#              #
#    Updated: 2022/08/29 18:35:01 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

TOKENIZATION_SRCS = tokenization.c expansion.c \
					tokenization2.c wildcard.c split_wildcard.c \
					print_tokens.c free_funcs.c tokenization3.c join_tokens.c

TOKENIZATION_SRCS := $(addprefix tokenization/, $(TOKENIZATION_SRCS))
COLOR=\033[0;34m
NC=\033[0m

GIGASHELL = \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣷⣤⣴⣾⣿⣿⣿⣿⣶⣿⣷⣮⣷⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⣿⣿⣿⢹⣿⡿⣿⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀  \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⣸⣿⡟⣸⣿⣷⡿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣷⣿⣿⣿⣡⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣞⣻⣷⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠲⠄⢈⡟⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⣿⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⡀⢻⣦⡿⠃⠀⢠⡀⡉⠉⠙⠻⣿⣿⣟⡧⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⣠⣼⡿⡏⣀⡀⣰⡼⣥⠀⠀⠀⠀⠸⣿⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣿⣿⣿⣿⣿⣿⣿⠛⠃⢠⡾⣻⣷⣿⣾⣿⣧⢸⡇⢀⠈⢀⡀⠀⠀⣧⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⠇⠀⠀⢠⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣀⣿⣇⠀⠀⢻⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣍⢘⣿⡏⠁⢾⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣏⣿⣤⡀⠈⠟⢿⣿⣿⣟⡛⠛⠿⢿⣿⣿⣿⡇⣿⣿⣿⣾⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⣿⣿⣿⣿⣿⣯⡀⠀⠸⣹⣿⣿⣷⣀⢀⣿⣿⠻⢿⣧⢹⠛⠛⡍⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠹⣿⣿⣿⣿⣿⣿⣶⣤⣭⣿⣿⣿⣿⣿⣿⡟⣿⣾⣿⡀⠀⠀⡅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡟⢰⣿⣟⢺⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⣻⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣟⠀⢸⣿⣥⣤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢻⣦⢰⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⠀⣾⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣍⡙⠛⣦⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣀⣀⠀⠀⣠⣾⠁⠈⣿⡟⢀⡙⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣾⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⣀⣀⣠⣤⣤⣤⣤⣤⣄⣀⣀⣤⣤⣶⣶⣿⣿⣿⣿⡛⠛⠻⠿⠿⣿⡿⠛⠁⠀⢸⣿⣿⡿⠁⢙⠙⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⢋⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣍⠉⡀⠉⠛⠿⣿⣶⣿⡏⠀⠀⠀⠀⢺⣿⣿⠀⠀⠈⣷⣦⣙⣿⣿⣿⣿⣿⣿⣷⣿⡿⡿⠏⣾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⣿⣿⠿⣛⡁⢤⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣼⣿⣿⣿⡿⠋⠀⢠⠄⢠⠀⢸⣿⣿⠀⠀⠀⢹⣿⣿⣿⣿⣿⠿⢿⣿⣿⣿⣿⣿⣾⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠛⠀⢀⣭⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣿⡇⠀⠠⡿⠀⢸⠀⢸⣿⣿⠀⠀⣸⣿⣿⣛⡛⠛⠀⠀⠀⠀⠀⠈⣉⠉⠀⠒⢤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⣶⠶⠿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⣸⣿⣿⣿⣿⣷⠀⠀⠀⠀⢸⡇⢸⣿⡏⢠⣾⣿⡿⠃⠉⠁⠀⠀⠀⠀⠀⠀⢰⣿⣄⣀⣄⠀⠈⠳⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⡟⠀⠈⠛⢻⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣤⡈⢻⣿⣿⣿⣿⣆⣀⡀⠀⣸⡇⠘⣿⣧⣿⡈⢻⣧⣄⣀⣾⡟⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣦⡀⠀⠈⠹⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠃⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⡿⣿⣿⣿⣿⣿⣷⣾⡏⠀⢠⣿⣿⣝⣇⡀⣧⣿⣿⣿⠀⠀⢀⠀⢀⣾⣿⣿⣿⣿⣿⡿⠻⣎⠄⠀⠈⢳⡀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⢸⣿⣿⣿⠇⢻⣿⣿⣿⣇⣠⣾⡟⠀⣾⣿⣿⣿⣿⡿⠟⠛⣽⣿⣿⣦⡀⠀⠹⣄⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⣠⣾⡿⣿⣿⢉⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣀⣿⣿⣿⣿⣶⣿⣿⠿⣿⠟⠛⠉⠀⠀⠚⠛⠛⠉⣁⠐⢠⡄⠈⠁⠀⠀⠐⢳⣄⠈⢷⡀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⡿⠁⢀⠼⢣⡟⢷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠻⣿⣿⡿⣿⣿⣿⡟⠋⠁⠀⠉⠙⣄⣀⠀⠀⠀⠀⠀⢠⣿⠀⢀⣠⡀⠀⠀⠀⠀⠀⠉⠦⡀⠙⠦⣤⡀⠀ \n \
\t\t\t\t\t\t\t ⣏⣀⠆⢰⣟⠀⢛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⣷⠀⠈⣿⣿⣿⣿⠟⢷⣦⣤⣰⣿⣶⡶⣦⣄⠀⠀⠀⠀⣼⡿⠀⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢦ \n \
\t\t\t\t\t\t\t ⣿⣿⡄⠸⠿⠬⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⣼⣿⣿⡄⢬⣀⠹⣿⣿⣿⣿⣿⢉⣿⠀⠀⠀⣰⣿⠟⣓⠀⠀⠦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t\t ⣿⣿⣧⣤⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣿⣯⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣤⣿⣾⣷⣬⣼⣷⣒⣀⣀⣀⣀⣀⣒⣂⣀⣀⣀ \n \
\t\t\t\t \n \033[0;31m\
\t\t\t ░██████╗░██╗░██████╗░░█████╗░░██████╗██╗░░██╗███████╗██╗░░░░░██╗░░░░░ ░█████╗░░█████╗░███╗░░░███╗██████╗░██╗██╗░░░░░███████╗██████╗░\n \
\t\t\t ██╔════╝░██║██╔════╝░██╔══██╗██╔════╝██║░░██║██╔════╝██║░░░░░██║░░░░░ ██╔══██╗██╔══██╗████╗░████║██╔══██╗██║██║░░░░░██╔════╝██╔══██╗\n \
\t\t\t ██║░░██╗░██║██║░░██╗░███████║╚█████╗░███████║█████╗░░██║░░░░░██║░░░░░ ██║░░╚═╝██║░░██║██╔████╔██║██████╔╝██║██║░░░░░█████╗░░██║░░██║\n \
\t\t\t ██║░░╚██╗██║██║░░╚██╗██╔══██║░╚═══██╗██╔══██║██╔══╝░░██║░░░░░██║░░░░░ ██║░░██╗██║░░██║██║╚██╔╝██║██╔═══╝░██║██║░░░░░██╔══╝░░██║░░██║\n \
\t\t\t ╚██████╔╝██║╚██████╔╝██║░░██║██████╔╝██║░░██║███████╗███████╗███████╗ ╚█████╔╝╚█████╔╝██║░╚═╝░██║██║░░░░░██║███████╗███████╗██████╔╝\n \
\t\t\t ░╚═════╝░╚═╝░╚═════╝░╚═╝░░╚═╝╚═════╝░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝ ░╚════╝░░╚════╝░╚═╝░░░░░╚═╝╚═╝░░░░░╚═╝╚══════╝╚══════╝╚═════╝░\n \

PARSING_SRCS = parsing.c parsing_utils.c
PARSING_SRCS := $(addprefix parsing/, $(PARSING_SRCS))

BUILT_INS_SRCS = env.c
BUILT_INS_SRCS := $(addprefix built_ins/, $(BUILT_INS_SRCS))

SRCS = $(TOKENIZATION_SRCS) $(PARSING_SRCS) $(BUILT_INS_SRCS)

SRCS += utils.c main.c

OBJS := $(SRCS:%.c=%.o)

NAME = minishell
CC = cc
CFLAGS = -Werror -Wall -Wextra -g
LIBFT = libft/libft.a


all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L/usr/local/lib -I/usr/local/include -lreadline $(LIBFT)
	@echo "$(COLOR)$(GIGASHELL)$(NC)"


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
	

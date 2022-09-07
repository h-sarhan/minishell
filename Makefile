# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/29 21:51:32 by hsarhan           #+#    #+#              #
#    Updated: 2022/09/07 16:35:06 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

TOKENIZATION_SRCS = tokenization.c expansion.c \
					tokenization2.c wildcard.c split_wildcard.c \
					print_tokens.c free_funcs.c tokenization3.c join_tokens.c

TOKENIZATION_SRCS := $(addprefix tokenization/, $(TOKENIZATION_SRCS))
COLOR=\033[0;34m
NC=\033[0m

GIGASHELL = \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣷⣤⣴⣾⣿⣿⣿⣿⣶⣿⣷⣮⣷⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⣿⣿⣿⢹⣿⡿⣿⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀  \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⣸⣿⡟⣸⣿⣷⡿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣷⣿⣿⣿⣡⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣞⣻⣷⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠲⠄⢈⡟⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⣿⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⡀⢻⣦⡿⠃⠀⢠⡀⡉⠉⠙⠻⣿⣿⣟⡧⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⣠⣼⡿⡏⣀⡀⣰⡼⣥⠀⠀⠀⠀⠸⣿⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣿⣿⣿⣿⣿⣿⣿⠛⠃⢠⡾⣻⣷⣿⣾⣿⣧⢸⡇⢀⠈⢀⡀⠀⠀⣧⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⠇⠀⠀⢠⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣀⣿⣇⠀⠀⢻⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣍⢘⣿⡏⠁⢾⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣏⣿⣤⡀⠈⠟⢿⣿⣿⣟⡛⠛⠿⢿⣿⣿⣿⡇⣿⣿⣿⣾⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⣿⣿⣿⣿⣿⣯⡀⠀⠸⣹⣿⣿⣷⣀⢀⣿⣿⠻⢿⣧⢹⠛⠛⡍⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠹⣿⣿⣿⣿⣿⣿⣶⣤⣭⣿⣿⣿⣿⣿⣿⡟⣿⣾⣿⡀⠀⠀⡅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡟⢰⣿⣟⢺⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⣻⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣟⠀⢸⣿⣥⣤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢻⣦⢰⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⠀⣾⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣍⡙⠛⣦⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣀⣀⣀⠀⠀⣠⣾⠁⠈⣿⡟⢀⡙⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣾⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⣀⣀⣠⣤⣤⣤⣤⣤⣄⣀⣀⣤⣤⣶⣶⣿⣿⣿⣿⡛⠛⠻⠿⠿⣿⡿⠛⠁⠀⢸⣿⣿⡿⠁⢙⠙⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⢋⣽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣍⠉⡀⠉⠛⠿⣿⣶⣿⡏⠀⠀⠀⠀⢺⣿⣿⠀⠀⠈⣷⣦⣙⣿⣿⣿⣿⣿⣿⣷⣿⡿⡿⠏⣾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⣿⣿⠿⣛⡁⢤⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣼⣿⣿⣿⡿⠋⠀⢠⠄⢠⠀⢸⣿⣿⠀⠀⠀⢹⣿⣿⣿⣿⣿⠿⢿⣿⣿⣿⣿⣿⣾⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠛⠀⢀⣭⣷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣿⡇⠀⠠⡿⠀⢸⠀⢸⣿⣿⠀⠀⣸⣿⣿⣛⡛⠛⠀⠀⠀⠀⠀⠈⣉⠉⠀⠒⢤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⣶⠶⠿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⣸⣿⣿⣿⣿⣷⠀⠀⠀⠀⢸⡇⢸⣿⡏⢠⣾⣿⡿⠃⠉⠁⠀⠀⠀⠀⠀⠀⢰⣿⣄⣀⣄⠀⠈⠳⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⡟⠀⠈⠛⢻⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣤⡈⢻⣿⣿⣿⣿⣆⣀⡀⠀⣸⡇⠘⣿⣧⣿⡈⢻⣧⣄⣀⣾⡟⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣦⡀⠀⠈⠹⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠃⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⡿⣿⣿⣿⣿⣿⣷⣾⡏⠀⢠⣿⣿⣝⣇⡀⣧⣿⣿⣿⠀⠀⢀⠀⢀⣾⣿⣿⣿⣿⣿⡿⠻⣎⠄⠀⠈⢳⡀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⢸⣿⣿⣿⠇⢻⣿⣿⣿⣇⣠⣾⡟⠀⣾⣿⣿⣿⣿⡿⠟⠛⣽⣿⣿⣦⡀⠀⠹⣄⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⣠⣾⡿⣿⣿⢉⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣀⣿⣿⣿⣿⣶⣿⣿⠿⣿⠟⠛⠉⠀⠀⠚⠛⠛⠉⣁⠐⢠⡄⠈⠁⠀⠀⠐⢳⣄⠈⢷⡀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⡿⠁⢀⠼⢣⡟⢷⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠻⣿⣿⡿⣿⣿⣿⡟⠋⠁⠀⠉⠙⣄⣀⠀⠀⠀⠀⠀⢠⣿⠀⢀⣠⡀⠀⠀⠀⠀⠀⠉⠦⡀⠙⠦⣤⡀⠀ \n \
\t\t\t\t\t\t ⣏⣀⠆⢰⣟⠀⢛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⣷⠀⠈⣿⣿⣿⣿⠟⢷⣦⣤⣰⣿⣶⡶⣦⣄⠀⠀⠀⠀⣼⡿⠀⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢦ \n \
\t\t\t\t\t\t ⣿⣿⡄⠸⠿⠬⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⣼⣿⣿⡄⢬⣀⠹⣿⣿⣿⣿⣿⢉⣿⠀⠀⠀⣰⣿⠟⣓⠀⠀⠦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⣿⣿⣧⣤⣤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣿⣯⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣤⣿⣾⣷⣬⣼⣷⣒⣀⣀⣀⣀⣀⣒⣂⣀⣀⣀ \n \
\t\t\t \n \033[0;31m\
\t\t ░██████╗░██╗░██████╗░░█████╗░░██████╗██╗░░██╗███████╗██╗░░░░░██╗░░░░░ ░█████╗░░█████╗░███╗░░░███╗██████╗░██╗██╗░░░░░███████╗██████╗░\n \
\t\t ██╔════╝░██║██╔════╝░██╔══██╗██╔════╝██║░░██║██╔════╝██║░░░░░██║░░░░░ ██╔══██╗██╔══██╗████╗░████║██╔══██╗██║██║░░░░░██╔════╝██╔══██╗\n \
\t\t ██║░░██╗░██║██║░░██╗░███████║╚█████╗░███████║█████╗░░██║░░░░░██║░░░░░ ██║░░╚═╝██║░░██║██╔████╔██║██████╔╝██║██║░░░░░█████╗░░██║░░██║\n \
\t\t ██║░░╚██╗██║██║░░╚██╗██╔══██║░╚═══██╗██╔══██║██╔══╝░░██║░░░░░██║░░░░░ ██║░░██╗██║░░██║██║╚██╔╝██║██╔═══╝░██║██║░░░░░██╔══╝░░██║░░██║\n \
\t\t ╚██████╔╝██║╚██████╔╝██║░░██║██████╔╝██║░░██║███████╗███████╗███████╗ ╚█████╔╝╚█████╔╝██║░╚═╝░██║██║░░░░░██║███████╗███████╗██████╔╝\n \
\t\t ░╚═════╝░╚═╝░╚═════╝░╚═╝░░╚═╝╚═════╝░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝ ░╚════╝░░╚════╝░╚═╝░░░░░╚═╝╚═╝░░░░░╚═╝╚══════╝╚══════╝╚═════╝░\n \

PARSING_SRCS = parsing.c parsing_utils.c
PARSING_SRCS := $(addprefix parsing/, $(PARSING_SRCS))

BUILTINS_SRCS = echo.c pwd.c builtins.c cd.c env.c export.c exit.c unset.c
BUILTINS_SRCS := $(addprefix builtins/, $(BUILTINS_SRCS))

EXEC_SRCS = exec.c
EXEC_SRCS := $(addprefix exec/, $(EXEC_SRCS))

SRCS = $(TOKENIZATION_SRCS) $(PARSING_SRCS) $(BUILTINS_SRCS) $(EXEC_SRCS)

SRCS += utils.c main.c

OBJS := $(SRCS:%.c=%.o)

NAME = minishell
CC = cc
CFLAGS = -Werror -Wall -Wextra -g -I/usr/local/opt/readline/include
LIBFT = libft/libft.a
FT_PRINTF = ft_printf/libftprintf.a


all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(FT_PRINTF)
	$(CC) $(CFLAGS)  $(OBJS) -o $(NAME)  $(LIBFT) $(FT_PRINTF)  -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline
	@echo "$(COLOR)$(GIGASHELL)$(NC)"


$(LIBFT):
	make -C libft

$(FT_PRINTF):
	make -C ft_printf


norm:
	norminette | grep Error

clean:
	rm -f $(OBJS)

fclean: clean
	make -C libft fclean
	make -C ft_printf fclean
	rm -f $(NAME)

re: fclean $(NAME)
	

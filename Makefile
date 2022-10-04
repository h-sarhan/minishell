# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/29 21:51:32 by hsarhan           #+#    #+#              #
#    Updated: 2022/10/04 09:22:01 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COLOR=\033[0;34m
NC=\033[0m
RED=\033[0;31m
LRED=\033[41m
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
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣿⣿⣿⣿⣿⣿⣿⠛⠃⢠⡾⣻⣷⣿$(RED)⢸⡇$(COLOR)⣧⢸⡇⢀⠈⢀⡀⠀⠀⣧⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⠇⠀⠀⢠⣶⣿⣿$(RED)⣠⣿⣿⣦$(COLOR)⣿⣿⣿⣀⣿$(RED)⢸⡇$(COLOR)⠀⠀⢻⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣍⢘⣿⡏⠁⢾⣷⣿⣿$(RED)⣤⣿⣿$(LRED)⣿⣿$(RED)⣿⣿⣤$(COLOR)⣿⡿$(RED)⣠⣿⣿⣦$(COLOR)⠦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣏⣿⣤⡀⠈⠟⢿⣿⣿⣟$(RED)⠛⣿⣿⠛$(COLOR)⣿⣿$(RED)⣤⣿⣿$(LRED)⣿⣿$(RED)⣿⣿⣤$(COLOR)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⣿⣿⣿⣿⣿⣯⡀⠀⠸⣹⣿⣿⣷$(RED)⢸⡇$(COLOR)⣿⣿⠻⢿⣧$(RED)⠛⣿⣿⠛$(COLOR)⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
\t\t\t\t\t\t ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠹⣿⣿⣿⣿⣿⣿⣶⣤⣭⣿⣿⣿⣿⣿⣿⡟⣿⣾⣿⡀$(RED)⢸⡇$(COLOR)⡅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n \
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

TOKENIZATION_SRCS = tokenize_line.c expansion.c \
					tokenize_subexpr.c wildcard.c split_wildcard.c \
					split_wildcard2.c free_funcs.c tokenize_quotes.c \
					contains_env_var.c wildcard_algorithm.c eat_quotes.c \
					tokenize_normal.c tokenize_env_var.c eat_dollars.c \
					tokenize_operator.c tokenize_line_utils.c tokenize_env_var_str.c

TOKENIZATION_SRCS := $(addprefix tokenization/, $(TOKENIZATION_SRCS))

PARSING_SRCS = parsing.c parsing_utils.c check_subexprs.c error_checking.c parse_step.c
PARSING_SRCS := $(addprefix parsing/, $(PARSING_SRCS))

BUILTINS_SRCS = echo.c pwd.c builtins.c env.c exit.c export.c export_utils.c \
				cd.c cd_utils.c unset.c unset_utils.c
BUILTINS_SRCS := $(addprefix builtins/, $(BUILTINS_SRCS))

EXEC_SRCS = exec_and_or.c exec_utils.c here_doc.c first_cmd.c mid_cmd.c cmd_utils.c exec_cmds.c wait_exit.c \
			exec_redir.c validate_redir.c validation_message.c handle_validation.c check_validation.c exec.c\
			exec_subexpr.c

EXEC_SRCS := $(addprefix exec/, $(EXEC_SRCS))

SIGNAL_SRCS = signal_handlers.c signals.c
SIGNAL_SRCS := $(addprefix signals/, $(SIGNAL_SRCS))

SRCS = $(TOKENIZATION_SRCS) $(PARSING_SRCS) $(BUILTINS_SRCS) $(EXEC_SRCS) $(SIGNAL_SRCS)

SRCS += utils.c main.c minishell.c

OBJS := $(SRCS:%.c=%.o)

NAME = minishell
CC = cc
CFLAGS = -Werror -Wall -Wextra -g -I/usr/local/opt/readline/include -Iincludes -Ift_printf -Ilibft
LIBFT = libft/libft.a
FT_PRINTF = ft_printf/libftprintf.a


all: $(NAME)

giga:
	@echo "$(COLOR)$(GIGASHELL)$(NC)"
	
$(NAME): $(OBJS) $(LIBFT) $(FT_PRINTF)
	$(CC) $(CFLAGS)  $(OBJS) -o $(NAME)  $(LIBFT) $(FT_PRINTF)  -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline
	@echo "$(COLOR)$(GIGASHELL)$(NC)"


$(LIBFT):
	make -C libft

$(FT_PRINTF):
	make -C ft_printf

bonus: $(NAME)

norm:
	norminette | grep Error

clean:
	rm -f $(OBJS)

fclean: clean
	make -C libft fclean
	make -C ft_printf fclean
	rm -f $(NAME)

re: fclean $(NAME)


# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/15 13:49:07 by hsarhan           #+#    #+#              #
#    Updated: 2022/06/29 15:01:11 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT_SRCS =	ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c \
				ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
				ft_itoa.c ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c ft_lstclear_bonus.c \
				ft_lstdelone_bonus.c ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c \
				ft_lstnew_bonus.c ft_lstsize_bonus.c ft_memchr.c ft_memcmp.c \
				ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c \
				ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c \
				ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c \
				ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c \
				ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c \
				ft_substr.c ft_tolower.c ft_toupper.c
				
LIBFT_SRCS := $(addprefix libft/, $(LIBFT_SRCS))

SRCS =  pipex.c utils.c utils2.c \
		error_handling.c split_args.c command.c \
		main.c run_commands.c here_doc.c

LIBFT_OBJS := $(LIBFT_SRCS:%.c=%.o)

OBJS := $(SRCS:%.c=%.o)

LIBFT = libft.a
CC = cc

CFLAGS = -Werror -Wextra -Wall -g3
NAME = pipex

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@

$(LIBFT): $(LIBFT_OBJS)
	@ar -rcs $(LIBFT) $(LIBFT_OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

norm:
	norminette $(SRCS) $(LIBFT_SRCS) libft/libft.h  pipex.h

bonus: $(NAME)

clean:
	@rm -f $(OBJS) $(LIBFT_OBJS) $(LIBFT)

fclean: clean
	@rm -f $(NAME)
	
re: fclean $(NAME)

.PHONY:  norm clean fclean re all bonus
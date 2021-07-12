# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yunslee <yunslee@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/10 02:48:31 by yunslee           #+#    #+#              #
#    Updated: 2021/07/13 04:05:11 by yunslee          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
FLAGS = -Wall -Wextra -Werror
SRCS = main.c
OBJS = $(SRCS:.c=.o)

./libft/libft.a:
	$(MAKE) -C ./libft/ all
	
$(NAME) : ./libft/libft.a
	gcc $(SRCS) -L ./libft -lft -o $(NAME)

all : $(NAME)
	gcc $(SRCS) -L ./libft -lft -o $(NAME)

clean :
	# $(MAKE) -C ./libft/ clean

fclean:
	$(MAKE) -C ./libft/ fclean
	rm -rf $(NAME);
re : clean all
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yunslee <yunslee@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/10 02:48:31 by yunslee           #+#    #+#              #
#    Updated: 2021/07/14 21:33:53 by yunslee          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror
NAME = pipex
FLAGS = -Wall -Wextra -Werror
SRCS = main.c
OBJS = $(SRCS:.c=.o)

all : $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

library:
	$(MAKE) -C ./libft/ all

$(NAME) : library $(OBJS)
	gcc $(OBJS) -L./libft -lft -o $(NAME)

clean :
	$(MAKE) -C ./libft/ clean
	rm -rf $(OBJS)

fclean: clean
	$(MAKE) -C ./libft/ fclean
	rm -rf $(NAME)
re : clean all
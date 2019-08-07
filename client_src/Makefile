# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmorer <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/05/20 11:35:32 by gmorer            #+#    #+#              #
#    Updated: 2019/07/04 22:02:13 by tet              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := client
CC := gcc
CFLAGS := -Werror -Wextra -Wall -g
HPATH = inc/
CPATH = src/
OPATH = obj/
INC = $(addprefix -I , $(HPATH))
CFILES =	main.c		\
			connect.c	\
			screen.c
OFILES = $(CFILES:.c=.o)
HFILES = inc/client.h
OBJ = $(addprefix $(OPATH), $(OFILES))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

$(OPATH)%.o: $(CPATH)%.c $(HFILES)
		mkdir -p $(OPATH)
		$(CC) -g $(INC) $< -c -o $@

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all

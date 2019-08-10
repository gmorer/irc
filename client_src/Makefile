# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmorer <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/05/20 11:35:32 by gmorer            #+#    #+#              #
#    Updated: 2019/08/10 10:58:53 by gmorer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := client
CC := gcc
CFLAGS := -Werror -Wextra -Wall -g
HPATH = inc/ ../libft/inc
CPATH = src/
OPATH = obj/
INC = $(addprefix -I , $(HPATH))
CFILES =	main.c		\
			connect.c	\
			screen.c	\
			command.c
OFILES = $(CFILES:.c=.o)
HFILES = inc/client.h
OBJ = $(addprefix $(OPATH), $(OFILES))
LIB = libft.a

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(OBJ) $(CFLAGS) -o $(NAME) $(LIB)

$(OPATH)%.o: $(CPATH)%.c $(HFILES)
		mkdir -p $(OPATH)
		$(CC) -g $(INC) $< -c -o $@

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all

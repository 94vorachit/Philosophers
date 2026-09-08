# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elwilks <elwilks@student.42bangkok.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/31 15:46:22 by elwilks           #+#    #+#              #
#    Updated: 2026/08/31 15:46:26 by elwilks          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = philo
CC      = cc
CFLAGS  = -Wall -Wextra -Werror

SRCS    = main.c \
			init.c \
			monitor.c \
			parse.c \
			setup.c \
			routine.c \
			routine_utils.c \
			check.c \
			utils.c \
			extra_utils.c

OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

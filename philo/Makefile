NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
FSAN = -g -O1 -fsanitize=address,undefined
CPPFLAGS = -Iincludes
SRCS = $(wildcard srcs/*.c)
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

fsan:
	$(MAKE) fclean
	$(MAKE) CFLAGS="$(CFLAGS) $(FSAN)"

.PHONY: all clean fclean re test fsan
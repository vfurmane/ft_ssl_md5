NAME		= ft_ssl
SRCS		= $(addprefix srcs/, main.c)
OBJS		= $(SRCS:.c=.o)

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -f

%.o:		%.c
	$(CC) $(CFLAGS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

.PHONY: all clean fclean re
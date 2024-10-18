NAME		= ft_ssl
SRCS		= $(addprefix srcs/, main.c \
				$(addprefix bits/, left_rotate.c reverse_endianness.c) \
				$(addprefix md5/, \
					$(addprefix hash/, algorithm.c) \
					$(addprefix message/, allocation.c)) \
				$(addprefix utils/, ft_bzero.c ft_memcpy.c ft_strlen.c))
INC_DIRS	= $(addprefix -I, includes/)
OBJS		= $(SRCS:.c=.o)

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -f

%.o:		%.c
	$(CC) $(CFLAGS) -c $< $(INC_DIRS) -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

.PHONY: all clean fclean re
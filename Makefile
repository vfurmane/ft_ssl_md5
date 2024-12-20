NAME		= ft_ssl
SRCS		= $(addprefix srcs/, main.c \
				$(addprefix args/, dispatch.c parse.c) \
				$(addprefix bits/, buffer_bswap_32.c left_rotate.c right_rotate.c) \
				$(addprefix md5/, \
					$(addprefix hash/, algorithm.c print.c) \
					$(addprefix message/, allocation.c)) \
				$(addprefix sha256/, \
					$(addprefix hash/, algorithm.c print.c) \
					$(addprefix message/, allocation.c)) \
				$(addprefix print/, binary.c hexadecimal.c md5.c sha256.c usage.c write.c) \
				$(addprefix utils/, ft_bswap.c ft_bzero.c ft_memchr.c ft_memcpy.c ft_min_max.c ft_strcmp.c ft_strlen.c))
INC_DIRS	= $(addprefix -I, includes/)
OBJS		= $(SRCS:.c=.o)

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
DEBUG_FLAGS	:=
RM			= rm -f
PYTEST		= pytest

%.o:		%.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< $(INC_DIRS) -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $^ -o $@

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

test:
	$(PYTEST)

.PHONY: all clean fclean re test
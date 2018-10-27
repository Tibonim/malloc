CC		= gcc

CFLAGS		+= -fPIC -Wall -Werror -Wextra $(LDFLAGS) -lpthread

LDFLAGS		= -Iinclude

NAME		= libmy_malloc.so

SRCS		= src/malloc_util.c		\
		  src/error.c			\
		  src/free.c			\
		  src/show_alloc_mem.c		\
		  src/realloc.c			\
		  src/malloc.c			\
		  src/calloc.c

OBJS		= $(SRCS:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
		$(CC) -shared -o $(NAME) $(OBJS)

clean:
		rm -f $(OBJS)

fclean:		clean
		rm -f $(NAME)

re:		fclean all

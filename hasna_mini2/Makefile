NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

SRC = helper_func_1.c \
      helper_func_2.c \
      helper_func_3.c \
      helper_func_4.c \
      libft.c \
      main.c 
OBJ = ${SRC:.c=.o}

all: ${NAME}

${NAME}: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -lreadline -o ${NAME}

clean:
	${RM} ${OBJ}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
.SECONDARY:

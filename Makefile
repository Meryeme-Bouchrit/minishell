NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm

SRC = mini.c
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

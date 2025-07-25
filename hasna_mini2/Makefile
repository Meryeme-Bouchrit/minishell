NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror 
RM = rm -f

SRC = a_get_type.c \
      array_to_tokens.c \
      libft2.c \
      a_token_list.c \
      libft.c \
      a_utils_1.c \
      main.c

OBJ = ${SRC:.c=.o}

all: ${NAME}

${NAME}: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -lreadline -o ${NAME}

clean :
	${RM} ${OBJ}

fclean : clean
	${RM} ${NAME}

re : fclean all

.PHONY : all clean fclean re
.SECONDARY:

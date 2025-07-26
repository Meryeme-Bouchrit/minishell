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
      expansion.c \
      main.c

OBJ = ${SRC:.c=.o}

all: ${NAME}

${NAME}: ${OBJ}
	@make -C libft_
	@${CC} ${CFLAGS} ${OBJ} -Llibft -lft -lreadline -o ${NAME}

clean :
	@make clean -C libft_
	@${RM} ${OBJ}

fclean : clean
	@make fclean -C libft_
	@${RM} ${NAME}

re : fclean all

.PHONY : all clean fclean re
.SECONDARY:

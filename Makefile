NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g
LDFLAGS		= -lreadline
RM			= rm -f

EXEC_DIR	= execution
PARSING_DIR	= parsing

SRC = 		\
			execution/built.c \
			execution/execution_1.c \
			execution/execution_utils1.c \
			execution/builtin_1.c \
			execution/builtin_2.c \
			execution/builtin_3.c \
			execution/builtin_utils_1.c \
			execution/builtin_utils_2.c \
			execution/builtin_utils_3.c \
			execution/builtin_utils_4.c \
			parsing/parser.c \
			parsing/a_get_type.c \
			parsing/array_to_tokens.c \
			parsing/a_token_list.c \
			parsing/libft.c \
			parsing/a_utils_1.c \
			parsing/expansion.c \
			parsing/correct_token.c \
			parsing/continue.c \
			parsing/my_strjoin.c \
			parsing/error_syntax.c \
			main.c

OBJ			= $(SRC:.c=.o)

INCLUDES	= -I. -I$(EXEC_DIR) -Ilibft -I$(PARSING_DIR)

LIBFT	= libft/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ)
	@make -C libft clean

fclean: clean
	$(RM) $(NAME)
	@make -C libft fclean

re: fclean all

.PHONY: all clean fclean re

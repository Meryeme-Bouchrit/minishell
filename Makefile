NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
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
			parsing/libft2.c \
			parsing/a_token_list.c \
			parsing/libft.c \
			parsing/a_utils_1.c \
			parsing/expansion.c \
			main.c

OBJ			= $(SRC:.c=.o)

INCLUDES	= -I. -I$(EXEC_DIR) -I$(EXEC_DIR)/libft -I$(PARSING_DIR) -I$(PARSING_DIR)/libft

LIBFT_EXEC	= $(EXEC_DIR)/libft/libft.a
LIBFT_PARSE	= $(PARSING_DIR)/libft/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(EXEC_DIR)/libft
	@make -C $(PARSING_DIR)/libft
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_EXEC) $(LIBFT_PARSE) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ)
	@make -C $(EXEC_DIR)/libft clean
	@make -C $(PARSING_DIR)/libft clean

fclean: clean
	$(RM) $(NAME)
	@make -C $(EXEC_DIR)/libft fclean
	@make -C $(PARSING_DIR)/libft fclean

re: fclean all

.PHONY: all clean fclean re

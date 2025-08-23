NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g
LDFLAGS     = -lreadline
RM          = rm -f

EXEC    = execution
BUILT   = builtin
PARS    = parsing
LIBFT   = libft/libft.a

# ===================== SOURCES ===================== #
SRC = \
	$(EXEC)/child_fork.c \
	$(EXEC)/exec_command.c \
	$(EXEC)/env_utils.c \
	$(EXEC)/heredoc_manage.c \
	$(EXEC)/heredoc_utils.c \
	$(EXEC)/path_utils.c \
	$(EXEC)/pipeline.c \
	$(EXEC)/pipe_setup.c \
	$(EXEC)/redir_utils.c \
	$(EXEC)/wait_utils.c \
	$(EXEC)/signals.c \
	$(BUILT)/builtin_core.c \
	$(BUILT)/builtin_1.c \
	$(BUILT)/builtin_2.c \
	$(BUILT)/builtin_3.c \
	$(BUILT)/builtin_env.c \
	$(BUILT)/utils.c \
	$(BUILT)/env_manage.c \
	$(BUILT)/env_lookup.c \
	$(PARS)/parser.c \
	$(PARS)/get_type.c \
	$(PARS)/free_cmds.c \
	$(PARS)/token_list.c \
	$(PARS)/libft.c \
	$(PARS)/utils.c \
	$(PARS)/expansion.c \
	$(PARS)/correct_token.c \
	$(PARS)/continue.c \
	$(PARS)/my_strjoin.c \
	$(PARS)/error_syntax.c \
	$(PARS)/continue_herdoc.c \
	main.c \
	main_utils.c

OBJ = $(SRC:.c=.o)
INCLUDES = -I. -I$(EXEC) -I$(BUILT) -Ilibft -I$(PARS)

# ===================== RULES ===================== #
all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ)
	@make -C libft clean

fclean: clean
	$(RM) $(NAME)
	@make -C libft fclean

re: fclean all

.PHONY: all clean fclean re

CC		=	gcc
NAME	=	minishell
FLAGS	=	-Wall -Wextra -Werror 
#FLAGS	+= -g3 
#FLAGS	+= -fsanitize=address 
#valgrind --leak-check=full --suppressions=valgrind.suppr ./minishell

SRC_PATH = ./srcs/
INC_PATH = ./include/
OBJ_PATH = ./obj/

SRC = 	init/init_utils.c \
		init/init.c \
		builtin/cd.c \
		builtin/echo.c \
		builtin/exit.c \
		builtin/export.c \
		builtin/export_bis.c \
		builtin/env.c \
		builtin/pwd.c \
		builtin/unset.c \
		parse/check_redir.c \
		parse/cmd_split.c \
		parse/ft_start.c \
		parse/if_doll.c \
		parse/is_redir.c \
		parse/no_quotes.c \
		parse/only_heredocs.c \
		parse/pipe_split.c \
		parse/quotes_redir.c \
		exec/run_cmd.c \
		exec/ft_pipe.c \
		exec/is_builtin.c \
		exec/exec_cmd.c \
		redir/ft_delim.c \
		redir/ft_heredocs.c \
		redir/ft_heredoc_utils.c \
		redir/ft_heredoc_utils2.c \
		redir/redir.c \
		utils/ft_list.c \
		utils/free.c \
		utils/utils.c \
		utils/protection.c \
		main/main.c \
		main/signal.c

SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJ	= $(SRC:.c=.o)
OBJS = $(addprefix $(OBJ_PATH), $(OBJ))
INC	= -I $(INC_PATH) -I $(LIBFT_PATH)

LIBFT_PATH = ./libft/
LIBFT	=	./libft/libft.a

all:		$(OBJ_PATH) $(LIBFT) $(NAME)

$(OBJ_PATH):
		mkdir -p $(OBJ_PATH)
		mkdir -p $(OBJ_PATH)/builtin
		mkdir -p $(OBJ_PATH)/init
		mkdir -p $(OBJ_PATH)/utils
		mkdir -p $(OBJ_PATH)/exec
		mkdir -p $(OBJ_PATH)/redir
		mkdir -p $(OBJ_PATH)/parse
		mkdir -p $(OBJ_PATH)/main

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
		$(CC) $(FLAGS) -c $< -o $@ $(INC)

$(NAME):	$(OBJS)
		$(CC) $(FLAGS) $(OBJS) -o $@ $(INC) $(LIBFT) -l readline

$(LIBFT):
		make -C $(LIBFT_PATH)

clean:
		make $@ -C $(LIBFT_PATH)
		rm -rf $(OBJ_PATH)

fclean:		clean
		$(MAKE) $@ -C $(LIBFT_PATH)
		rm -rf $(NAME)

re:			fclean all

.PHONY: all clean fclean re

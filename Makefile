NAME        = cub3D

CC          = cc
CFLAGS      = -Wall -Wextra -Werror

SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = includes
LIBFT_DIR   = libft

INCLUDES    = -I$(INC_DIR) -I$(LIBFT_DIR) -I/usr/local/include

LIBFT       = $(LIBFT_DIR)/libft.a
LIBS        = $(LIBFT) -L/usr/local/lib -lmlx -lXext -lX11 -lm

SRCS        = $(shell find $(SRC_DIR) -name "*.c")
OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

SRC := main.c
	
OBJ := $(SRC:.c=.o)

CC := cc
CFLAGS := -Wall -Wextra -Werror
INCLUDES := -Imlx -I Libft/include -I ft_printf/include -I ft_gnl/include
LDLIBS := -lmlx -lXext -lX11 -lm

NAME := cub3d
LIBFT_DIR := Libft
LIBFT := $(LIBFT_DIR)/libft.a
FT_PRINTF_DIR := ft_printf
FT_PRINTF := $(FT_PRINTF_DIR)/libftprintf.a
FT_GNL_DIR := ft_gnl
FT_GNL := $(FT_GNL_DIR)/gnl.a

all: $(LIBFT) $(FT_PRINTF) $(FT_GNL) $(NAME)

$(LIBFT):
			@$(MAKE) -C $(LIBFT_DIR)

$(FT_PRINTF):
			@$(MAKE) -C $(FT_PRINTF_DIR)

$(FT_GNL):
			@$(MAKE) -C $(FT_GNL_DIR)

%.o: %.c
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJ)
			$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(FT_PRINTF) $(FT_GNL) $(LSLIBS) -o $(NAME)

clean:
			@rm -f $(OBJ)
			@$(MAKE) -C $(FT_GNL_DIR) clean
			@$(MAKE) -C $(FT_PRINTF_DIR) clean
			@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
			@rm -f $(NAME)
			@$(MAKE) -C $(FT_GNL_DIR) fclean
			@$(MAKE) -C $(FT_PRINTF_DIR) fclean
			@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

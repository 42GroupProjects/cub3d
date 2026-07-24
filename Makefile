NAME        = cub3D
PARSE_NAME  = cub3D_parse
MLX_NAME    = cub3D_mlxtest

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

VALID_MAPS  = $(wildcard maps/m_valid/*.cub)
INVALID_MAPS = $(wildcard maps/m_invalid/*.cub)

VALGRIND    = valgrind --leak-check=full --show-leak-kinds=all \
		--error-exitcode=42 --track-origins=yes

all: $(NAME)

bonus: all

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
	rm -f $(NAME) $(PARSE_NAME) $(MLX_NAME)

debug: CFLAGS += -g
debug: re

demo: all
	./$(NAME) maps/demo.cub

# Test binaries use compile flags (getenv is forbidden by the subject).
parse-bin: $(LIBFT)
	$(CC) $(CFLAGS) -DCUB3D_PARSE_ONLY $(INCLUDES) $(SRCS) $(LIBS) -o $(PARSE_NAME)

mlx-bin: $(LIBFT)
	$(CC) $(CFLAGS) -DCUB3D_QUIT_AFTER_INIT $(INCLUDES) $(SRCS) $(LIBS) -o $(MLX_NAME)

test-maps: parse-bin
	@fail=0; \
	for f in $(INVALID_MAPS); do \
		echo "== invalid $$f =="; \
		if ./$(PARSE_NAME) "$$f" >/dev/null 2>err.tmp; then \
			echo "FAIL: expected nonzero exit for $$f"; fail=1; \
		elif ! grep -q '^Error' err.tmp; then \
			echo "FAIL: expected Error on stderr for $$f"; fail=1; \
		else \
			echo "OK"; \
		fi; \
	done; \
	for f in $(VALID_MAPS); do \
		echo "== valid $$f =="; \
		if ! ./$(PARSE_NAME) "$$f" >/dev/null 2>err.tmp; then \
			echo "FAIL: expected success for $$f"; cat err.tmp; fail=1; \
		else \
			echo "OK"; \
		fi; \
	done; \
	rm -f err.tmp; \
	if [ $$fail -ne 0 ]; then exit 1; fi; \
	echo "test-maps: all passed"

valgrind-parser: parse-bin
	@fail=0; \
	for f in $(INVALID_MAPS); do \
		echo "== valgrind invalid $$f =="; \
		$(VALGRIND) ./$(PARSE_NAME) "$$f" >/dev/null 2>vg.tmp; \
		rc=$$?; \
		if [ $$rc -eq 42 ]; then \
			echo "FAIL: valgrind reported issues for $$f"; cat vg.tmp; fail=1; \
		elif [ $$rc -eq 0 ]; then \
			echo "FAIL: expected nonzero exit for $$f"; fail=1; \
		elif ! grep -q '^Error' vg.tmp; then \
			echo "FAIL: expected Error on stderr for $$f"; fail=1; \
		else \
			echo "OK"; \
		fi; \
	done; \
	for f in $(VALID_MAPS); do \
		echo "== valgrind valid $$f =="; \
		$(VALGRIND) ./$(PARSE_NAME) "$$f" >/dev/null 2>vg.tmp; \
		rc=$$?; \
		if [ $$rc -eq 42 ]; then \
			echo "FAIL: valgrind reported issues for $$f"; cat vg.tmp; fail=1; \
		elif [ $$rc -ne 0 ]; then \
			echo "FAIL: expected success for $$f (rc=$$rc)"; cat vg.tmp; fail=1; \
		else \
			echo "OK"; \
		fi; \
	done; \
	rm -f vg.tmp; \
	if [ $$fail -ne 0 ]; then exit 1; fi; \
	echo "valgrind-parser: all passed"

valgrind-mlx: mlx-bin
	@echo "== valgrind MLX teardown maps/m_valid/01_minimal_N.cub =="; \
	if command -v xvfb-run >/dev/null 2>&1; then \
		xvfb-run -a $(VALGRIND) \
			./$(MLX_NAME) maps/m_valid/01_minimal_N.cub >/dev/null 2>vg.mlx.tmp; \
	else \
		$(VALGRIND) \
			./$(MLX_NAME) maps/m_valid/01_minimal_N.cub >/dev/null 2>vg.mlx.tmp; \
	fi; \
	rc=$$?; \
	if [ $$rc -eq 42 ]; then \
		echo "FAIL: valgrind reported issues"; cat vg.mlx.tmp; rm -f vg.mlx.tmp; exit 1; \
	elif [ $$rc -ne 0 ]; then \
		echo "FAIL: expected success (rc=$$rc)"; cat vg.mlx.tmp; rm -f vg.mlx.tmp; exit 1; \
	fi; \
	rm -f vg.mlx.tmp; \
	echo "valgrind-mlx: OK (ignore X11 still-reachable if any in manual runs)"

re: fclean all

.PHONY: all bonus clean fclean re debug demo parse-bin mlx-bin \
	test-maps valgrind-parser valgrind-mlx

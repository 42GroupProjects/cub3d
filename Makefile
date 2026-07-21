NAME        = cub3d

CC          = cc
CFLAGS      = -Wall -Wextra -Werror

SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = includes
LIBFT_DIR   = libft

INCLUDES    = -I$(INC_DIR) -I$(LIBFT_DIR) -I/usr/local/include

LIBFT       = $(LIBFT_DIR)/libft.a
LIBS        = $(LIBFT) -L/usr/local/lib -lmlx -lXext -lX11 -lm

SRCS 		= $(shell find $(SRC_DIR) -name "*.c")

OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

VALID_MAPS  = $(wildcard maps/m_valid/*.cub)
INVALID_MAPS = $(wildcard maps/m_invalid/*.cub)

VALGRIND    = valgrind --leak-check=full --show-leak-kinds=all \
		--error-exitcode=42 --track-origins=yes

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

debug: CFLAGS += -g
debug: re

demo: all
	./$(NAME) maps/demo.cub

# Parse-only map suites (no MLX). Invalid must fail; valid must succeed.
test-maps: $(NAME)
	@fail=0; \
	for f in $(INVALID_MAPS); do \
		echo "== invalid $$f =="; \
		if CUB3D_PARSE_ONLY=1 ./$(NAME) "$$f" >/dev/null 2>err.tmp; then \
			echo "FAIL: expected nonzero exit for $$f"; fail=1; \
		elif ! grep -q '^Error' err.tmp; then \
			echo "FAIL: expected Error on stderr for $$f"; fail=1; \
		else \
			echo "OK"; \
		fi; \
	done; \
	for f in $(VALID_MAPS); do \
		echo "== valid $$f =="; \
		if ! CUB3D_PARSE_ONLY=1 ./$(NAME) "$$f" >/dev/null 2>err.tmp; then \
			echo "FAIL: expected success for $$f"; cat err.tmp; fail=1; \
		else \
			echo "OK"; \
		fi; \
	done; \
	rm -f err.tmp; \
	if [ $$fail -ne 0 ]; then exit 1; fi; \
	echo "test-maps: all passed"

# Same suites under valgrind. Exit 42 from valgrind = leak/error.
valgrind-parser: $(NAME)
	@fail=0; \
	for f in $(INVALID_MAPS); do \
		echo "== valgrind invalid $$f =="; \
		CUB3D_PARSE_ONLY=1 $(VALGRIND) ./$(NAME) "$$f" >/dev/null 2>vg.tmp; \
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
		CUB3D_PARSE_ONLY=1 $(VALGRIND) ./$(NAME) "$$f" >/dev/null 2>vg.tmp; \
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

# MLX init + clean_exit under valgrind (needs X11; use xvfb-run in CI/headless).
valgrind-mlx: $(NAME)
	@echo "== valgrind MLX teardown maps/m_valid/01_minimal_N.cub =="; \
	if command -v xvfb-run >/dev/null 2>&1; then \
		xvfb-run -a env CUB3D_QUIT_AFTER_INIT=1 $(VALGRIND) \
			./$(NAME) maps/m_valid/01_minimal_N.cub >/dev/null 2>vg.mlx.tmp; \
	else \
		CUB3D_QUIT_AFTER_INIT=1 $(VALGRIND) \
			./$(NAME) maps/m_valid/01_minimal_N.cub >/dev/null 2>vg.mlx.tmp; \
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

.PHONY: all clean fclean re debug demo test-maps valgrind-parser valgrind-mlx

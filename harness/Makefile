# Local-only test harness. Not part of the subject submission.
# Usage from repo root:  make -C harness test-maps
#                        make -C harness valgrind-parser
#                        make -C harness valgrind-mlx

ROOT        = ..
PARSE_NAME  = $(ROOT)/cub3D_parse
MLX_NAME    = $(ROOT)/cub3D_mlxtest

CC          = cc
CFLAGS      = -Wall -Wextra -Werror

SRC_DIR     = $(ROOT)/src
INC_DIR     = $(ROOT)/includes
LIBFT_DIR   = $(ROOT)/libft

INCLUDES    = -I$(INC_DIR) -I$(LIBFT_DIR) -I/usr/local/include
LIBFT       = $(LIBFT_DIR)/libft.a
LIBS        = $(LIBFT) -L/usr/local/lib -lmlx -lXext -lX11 -lm

GAME_SRCS   = $(filter-out $(SRC_DIR)/main.c,$(shell find $(SRC_DIR) -name "*.c"))

VALID_MAPS  = $(wildcard $(ROOT)/maps/m_valid/*.cub)
INVALID_MAPS = $(wildcard $(ROOT)/maps/m_invalid/*.cub)

VALGRIND    = valgrind --leak-check=full --show-leak-kinds=all \
		--error-exitcode=42 --track-origins=yes

all: test-maps

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

parse-bin: $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(GAME_SRCS) main_parse.c $(LIBS) \
		-o $(PARSE_NAME)

mlx-bin: $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(GAME_SRCS) main_mlx.c $(LIBS) \
		-o $(MLX_NAME)

test-maps: parse-bin
	@fail=0; \
	for f in $(INVALID_MAPS); do \
		echo "== invalid $$f =="; \
		if $(PARSE_NAME) "$$f" >/dev/null 2>$(ROOT)/err.tmp; then \
			echo "FAIL: expected nonzero exit for $$f"; fail=1; \
		elif ! grep -q '^Error' $(ROOT)/err.tmp; then \
			echo "FAIL: expected Error on stderr for $$f"; fail=1; \
		else \
			echo "OK"; \
		fi; \
	done; \
	for f in $(VALID_MAPS); do \
		echo "== valid $$f =="; \
		if ! $(PARSE_NAME) "$$f" >/dev/null 2>$(ROOT)/err.tmp; then \
			echo "FAIL: expected success for $$f"; cat $(ROOT)/err.tmp; fail=1; \
		else \
			echo "OK"; \
		fi; \
	done; \
	rm -f $(ROOT)/err.tmp; \
	if [ $$fail -ne 0 ]; then exit 1; fi; \
	echo "test-maps: all passed"

valgrind-parser: parse-bin
	@fail=0; \
	for f in $(INVALID_MAPS); do \
		echo "== valgrind invalid $$f =="; \
		$(VALGRIND) $(PARSE_NAME) "$$f" >/dev/null 2>$(ROOT)/vg.tmp; \
		rc=$$?; \
		if [ $$rc -eq 42 ]; then \
			echo "FAIL: valgrind reported issues for $$f"; cat $(ROOT)/vg.tmp; fail=1; \
		elif [ $$rc -eq 0 ]; then \
			echo "FAIL: expected nonzero exit for $$f"; fail=1; \
		elif ! grep -q '^Error' $(ROOT)/vg.tmp; then \
			echo "FAIL: expected Error on stderr for $$f"; fail=1; \
		else \
			echo "OK"; \
		fi; \
	done; \
	for f in $(VALID_MAPS); do \
		echo "== valgrind valid $$f =="; \
		$(VALGRIND) $(PARSE_NAME) "$$f" >/dev/null 2>$(ROOT)/vg.tmp; \
		rc=$$?; \
		if [ $$rc -eq 42 ]; then \
			echo "FAIL: valgrind reported issues for $$f"; cat $(ROOT)/vg.tmp; fail=1; \
		elif [ $$rc -ne 0 ]; then \
			echo "FAIL: expected success for $$f (rc=$$rc)"; cat $(ROOT)/vg.tmp; fail=1; \
		else \
			echo "OK"; \
		fi; \
	done; \
	rm -f $(ROOT)/vg.tmp; \
	if [ $$fail -ne 0 ]; then exit 1; fi; \
	echo "valgrind-parser: all passed"

valgrind-mlx: mlx-bin
	@echo "== valgrind MLX teardown maps/m_valid/01_minimal_N.cub =="; \
	if command -v xvfb-run >/dev/null 2>&1; then \
		xvfb-run -a $(VALGRIND) \
			$(MLX_NAME) $(ROOT)/maps/m_valid/01_minimal_N.cub \
			>/dev/null 2>$(ROOT)/vg.mlx.tmp; \
	else \
		$(VALGRIND) \
			$(MLX_NAME) $(ROOT)/maps/m_valid/01_minimal_N.cub \
			>/dev/null 2>$(ROOT)/vg.mlx.tmp; \
	fi; \
	rc=$$?; \
	if [ $$rc -eq 42 ]; then \
		echo "FAIL: valgrind reported issues"; cat $(ROOT)/vg.mlx.tmp; \
		rm -f $(ROOT)/vg.mlx.tmp; exit 1; \
	elif [ $$rc -ne 0 ]; then \
		echo "FAIL: expected success (rc=$$rc)"; cat $(ROOT)/vg.mlx.tmp; \
		rm -f $(ROOT)/vg.mlx.tmp; exit 1; \
	fi; \
	rm -f $(ROOT)/vg.mlx.tmp; \
	echo "valgrind-mlx: OK (ignore X11 still-reachable if any in manual runs)"

clean:
	rm -f $(PARSE_NAME) $(MLX_NAME) $(ROOT)/err.tmp $(ROOT)/vg.tmp \
		$(ROOT)/vg.mlx.tmp

fclean: clean

re: fclean all

.PHONY: all clean fclean re parse-bin mlx-bin \
	test-maps valgrind-parser valgrind-mlx

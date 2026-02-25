##
# Monty Stack/Queue Interpreter
# A robust C implementation with library support
#

# Compiler and flags
CC				= gcc
CFLAGS_BASE		= -Wall -Werror -Wextra -pedantic -fPIC
CFLAGS_DEBUG	= $(CFLAGS_BASE) -g -O0 -DDEBUG
CFLAGS_RELEASE	= $(CFLAGS_BASE) -O2 -DNDEBUG
CFLAGS_PROFILE	= $(CFLAGS_BASE) -g -p
CFLAGS			= $(CFLAGS_RELEASE)

# Library and executable names
NAME			= monty
LIB_STATIC		= libmonty.a
LIB_SHARED		= libmonty.so
LIB_VERSION		= 1.0.0

# Directories
SRC_DIR			= src
OBJ_DIR			= build/obj
LIB_DIR			= build/lib
BIN_DIR			= build/bin
DIST_DIR		= dist

# Source files - Main interpreter
MAIN_SRC		= monty_main.c
MAIN_OBJ		= $(OBJ_DIR)/monty_main.o

# Library source files (excludes main)
LIB_SRCS		= exec.c \
				  monty_operators.c \
				  monty_operators2.c \
				  monty_operators3.c \
				  monty_operators4.c \
				  node_addition.c \
				  queue.c \
				  pchar.c \
				  p_str.c \
				  rotl.c \
				  rotr.c \
				  freeStack.c \
				  monty_lib.c

LIB_OBJS		= $(LIB_SRCS:%.c=$(OBJ_DIR)/%.o)
ALL_OBJS		= $(MAIN_OBJ) $(LIB_OBJS)

# Phony targets
.PHONY: all clean fclean re help
.PHONY: binary static-lib shared-lib libs
.PHONY: debug release profile
.PHONY: install dist example test

# Default target
all: binary

# ============================================================================
# BUILDING
# ============================================================================

# Build main executable (default)
binary: CFLAGS := $(CFLAGS_RELEASE)
binary: $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(ALL_OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "[LINK] $@"
	@$(CC) $(CFLAGS) $^ -o $@

# Build static library
static-lib: CFLAGS := $(CFLAGS_RELEASE)
static-lib: $(LIB_DIR)/$(LIB_STATIC)

$(LIB_DIR)/$(LIB_STATIC): $(LIB_OBJS)
	@mkdir -p $(LIB_DIR)
	@echo "[AR] $@"
	@ar rcs $@ $^

# Build shared library
shared-lib: CFLAGS := $(CFLAGS_RELEASE)
shared-lib: $(LIB_DIR)/$(LIB_SHARED)

$(LIB_DIR)/$(LIB_SHARED): $(LIB_OBJS)
	@mkdir -p $(LIB_DIR)
	@echo "[LINK] $@"
	@$(CC) -shared $(CFLAGS) $^ -o $@

# Build both static and shared libraries
libs: static-lib shared-lib
	@echo "[INFO] Both libraries built successfully"

# Compile object files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# ============================================================================
# DEBUG AND PROFILE BUILDS
# ============================================================================

debug: CFLAGS := $(CFLAGS_DEBUG)
debug: fclean binary
	@echo "[INFO] Debug build complete"

release: CFLAGS := $(CFLAGS_RELEASE)
release: fclean binary
	@echo "[INFO] Release build complete"

profile: CFLAGS := $(CFLAGS_PROFILE)
profile: fclean binary
	@echo "[INFO] Profile build complete"

# ============================================================================
# CLEANING
# ============================================================================

clean:
	@echo "[CLEAN] Object files"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "[FCLEAN] All build artifacts"
	@rm -rf build/ dist/ *.so *.a

re: fclean all
	@echo "[INFO] Rebuild complete"

# ============================================================================
# INSTALLATION AND DISTRIBUTION
# ============================================================================

install: libs
	@echo "[INSTALL] Installing libraries and headers"
	@mkdir -p /usr/local/include/monty
	@mkdir -p /usr/local/lib
	@cp monty.h /usr/local/include/monty/monty.h
	@cp monty_public.h /usr/local/include/monty/monty_public.h
	@cp $(LIB_DIR)/$(LIB_STATIC) /usr/local/lib/
	@cp $(LIB_DIR)/$(LIB_SHARED) /usr/local/lib/
	@ldconfig
	@echo "[INFO] Installation complete"
	@echo "[INFO] Headers: /usr/local/include/monty/"
	@echo "[INFO] Libraries: /usr/local/lib/"

uninstall:
	@echo "[UNINSTALL] Removing installed files"
	@rm -rf /usr/local/include/monty
	@rm -f /usr/local/lib/$(LIB_STATIC)
	@rm -f /usr/local/lib/$(LIB_SHARED)
	@ldconfig
	@echo "[INFO] Uninstallation complete"

dist: fclean libs binary
	@echo "[DIST] Creating distribution package"
	@mkdir -p $(DIST_DIR)/monty-$(LIB_VERSION)
	@cp -r *.c *.h Makefile README.md $(DIST_DIR)/monty-$(LIB_VERSION)/
	@cp -r test/*.m $(DIST_DIR)/monty-$(LIB_VERSION)/ 2>/dev/null || true
	@mkdir -p $(DIST_DIR)/monty-$(LIB_VERSION)/lib
	@cp $(LIB_DIR)/* $(DIST_DIR)/monty-$(LIB_VERSION)/lib/
	@mkdir -p $(DIST_DIR)/monty-$(LIB_VERSION)/bin
	@cp $(BIN_DIR)/$(NAME) $(DIST_DIR)/monty-$(LIB_VERSION)/bin/
	@cd $(DIST_DIR) && tar czf monty-$(LIB_VERSION).tar.gz monty-$(LIB_VERSION)/
	@echo "[INFO] Distribution created: $(DIST_DIR)/monty-$(LIB_VERSION).tar.gz"

# ============================================================================
# EXAMPLES AND TESTING
# ============================================================================

example: binary
	@echo "[EXAMPLE] Running example bytecode"
	@mkdir -p test
	@echo "push 1" > test/example.m
	@echo "push 2" >> test/example.m
	@echo "add" >> test/example.m
	@echo "pint" >> test/example.m
	@$(BIN_DIR)/$(NAME) test/example.m

test: binary
	@echo "[TEST] Running tests"
	@mkdir -p test
	@echo "push 10" > test/test1.m
	@echo "pint" >> test/test1.m
	@echo "Running test1.m: push 10, pint"
	@$(BIN_DIR)/$(NAME) test/test1.m
	@echo ""
	@echo "push 5" > test/test2.m
	@echo "push 3" >> test/test2.m
	@echo "add" >> test/test2.m
	@echo "pint" >> test/test2.m
	@echo "Running test2.m: push 5, push 3, add, pint"
	@$(BIN_DIR)/$(NAME) test/test2.m
	@echo ""
	@echo "push 1" > test/test3.m
	@echo "push 2" >> test/test3.m
	@echo "swap" >> test/test3.m
	@echo "pall" >> test/test3.m
	@echo "Running test3.m: push 1, push 2, swap, pall"
	@$(BIN_DIR)/$(NAME) test/test3.m

# ============================================================================
# INFO AND HELP
# ============================================================================

help:
	@echo "╔════════════════════════════════════════════════════════════════╗"
	@echo "║           Monty Stack/Queue Interpreter - Build System         ║"
	@echo "╚════════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "COMPILATION TARGETS:"
	@echo "  make              - Build main executable (release mode)"
	@echo "  make binary       - Build executable only"
	@echo "  make static-lib   - Build static library (.a)"
	@echo "  make shared-lib   - Build shared library (.so)"
	@echo "  make libs         - Build both static and shared libraries"
	@echo ""
	@echo "BUILD VARIANTS:"
	@echo "  make debug        - Build with debug symbols and no optimization"
	@echo "  make release      - Build optimized release version"
	@echo "  make profile      - Build with profiling support"
	@echo ""
	@echo "CLEANING TARGETS:"
	@echo "  make clean        - Remove object files"
	@echo "  make fclean       - Remove all build artifacts"
	@echo "  make re           - Clean rebuild"
	@echo ""
	@echo "INSTALLATION:"
	@echo "  make install      - Install libraries and headers to /usr/local/"
	@echo "  make uninstall    - Remove installed files"
	@echo "  make dist         - Create distribution package"
	@echo ""
	@echo "TESTING:"
	@echo "  make example      - Run a simple example"
	@echo "  make test         - Run test suite"
	@echo ""
	@echo "OTHER:"
	@echo "  make help         - Display this help message"
	@echo ""
	@echo "VARIABLES:"
	@echo "  CC                - Compiler (default: gcc)"
	@echo "  CFLAGS            - Compiler flags"
	@echo ""
	@echo "EXAMPLES:"
	@echo "  make                          # Build release binary"
	@echo "  make debug                    # Build with debug symbols"
	@echo "  make libs                     # Build libraries"
	@echo "  make install                  # Install to system"
	@echo "  make dist                     # Create distribution tarball"
	@echo ""

info:
	@echo "Build Configuration:"
	@echo "  Compiler: $(CC)"
	@echo "  Flags: $(CFLAGS)"
	@echo "  Binary: $(BIN_DIR)/$(NAME)"
	@echo "  Static Lib: $(LIB_DIR)/$(LIB_STATIC)"
	@echo "  Shared Lib: $(LIB_DIR)/$(LIB_SHARED)"
	@echo ""
	@echo "Object files: $(ALL_OBJS)"

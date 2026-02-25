# Monty Stack/Queue Interpreter

A C implementation of a bytecode interpreter that processes stack and queue operations, supporting dynamic switching between LIFO (Last In, First Out) and FIFO (First In, First Out) modes.

## Table of Contents

- [Overview](#overview)
- [Compilation](#compilation)
- [Usage](#usage)
- [Operations Reference](#operations-reference)
- [File Format](#file-format)
- [Examples](#examples)
- [Project Structure](#project-structure)
- [Data Structures](#data-structures)

## Overview

The Monty interpreter reads bytecode files and executes a series of stack manipulation operations. It provides:

- **Dynamic data structure switching**: Toggle between stack (LIFO) and queue (FIFO) at runtime
- **Arithmetic operations**: Add, subtract, multiply, divide, modulo
- **I/O operations**: Print integers and ASCII characters
- **Stack manipulation**: Push, pop, swap, rotate
- **Error handling**: Comprehensive error messages with line numbers

## Compilation

### Prerequisites
- GCC compiler
- Standard C library (glibc)
- POSIX-compliant system
- `make` utility

### Quick Start with Makefile

```bash
# Build release binary
make

# View all available targets
make help

# Other useful targets
make debug           # Build with debug symbols
make release         # Build optimized version
make libs            # Build static and shared libraries
```

### Manual Compilation (Without Makefile)

```bash
# Standard compilation
gcc -Wall -Werror -Wextra -pedantic *.c -o monty

# With debug symbols
gcc -Wall -Werror -Wextra -pedantic -g *.c -o monty

# With optimization
gcc -Wall -Werror -Wextra -pedantic -O2 *.c -o monty
```

## Usage

### Basic Syntax

```bash
./monty <bytecode_file>
```

### Parameters

- `<bytecode_file>`: Path to a text file containing Monty bytecode operations

### Error Handling

The interpreter provides detailed error messages:
- **L[line_number]: unknown instruction [opcode]** - Invalid operation
- **L[line_number]: usage: push integer** - Invalid push argument
- **L[line_number]: can't [operation], stack too short** - Insufficient stack elements
- **Error: Can't open file [filename]** - File not found or unreadable

## Operations Reference

### Stack Manipulation

| Operation | Arguments | Description |
|-----------|-----------|-------------|
| `push` | integer | Push integer onto stack/queue |
| `pop` | none | Remove top element from stack |
| `pall` | none | Print all elements in stack/queue (top to bottom) |
| `pint` | none | Print top element of stack |
| `swap` | none | Swap top two elements |
| `rotl` | none | Rotate stack left (move top to bottom) |
| `rotr` | none | Rotate stack right (move bottom to top) |
| `nop` | none | No operation (do nothing) |

### Arithmetic Operations

| Operation | Arguments | Description |
|-----------|-----------|-------------|
| `add` | none | Pop top two elements, push their sum |
| `sub` | none | Pop top two, push (second - top) |
| `mul` | none | Pop top two elements, push their product |
| `div` | none | Pop top two, push (second / top) |
| `mod` | none | Pop top two, push (second % top) |

### Output Operations

| Operation | Arguments | Description |
|-----------|-----------|-------------|
| `pchar` | none | Print top element as ASCII character |
| `pstr` | none | Print string from stack (ASCII values until 0) |

### Mode Control

| Operation | Arguments | Description |
|-----------|-----------|-------------|
| `stack` | none | Switch to stack mode (LIFO) - default |
| `queue` | none | Switch to queue mode (FIFO) |

## File Format

Bytecode files contain one operation per line. Comments start with `#` and are ignored.

### Example File Structure

```
# This is a comment
push 1
push 2
push 3
pall
add
pint
```

## Examples

### Example 1: Simple Push and Print

**File: test1.m**
```
push 42
pint
```

**Execution:**
```bash
./monty test1.m
```

**Output:**
```
42
```

### Example 2: Arithmetic Operations

**File: test_math.m**
```
push 10
push 5
add
pint
```

**Execution:**
```bash
./monty test_math.m
```

**Output:**
```
15
```

### Example 3: Stack Mode Operations

**File: test_stack.m**
```
push 1
push 2
push 3
pall
swap
pall
pop
pall
```

**Execution:**
```bash
./monty test_stack.m
```

**Output:**
```
3
2
1
2
3
1
2
1
```

### Example 4: Queue Mode

**File: test_queue.m**
```
queue
push 1
push 2
push 3
pall
```

**Execution:**
```bash
./monty test_queue.m
```

**Output:**
```
1
2
3
```

### Example 5: Character Output

**File: test_char.m**
```
push 72
push 101
push 108
push 108
push 111
pstr
```

**Execution:**
```bash
./monty test_char.m
```

**Output:**
```
olleH
```

## Project Structure

```
monty/
├── README.md                    # Main documentation
├── LIBRARY.md                   # Library usage guide
├── Makefile                     # Build system
├── monty.h                      # Internal header
├── monty_public.h               # Public library header
├── monty_main.c                 # Entry point
├── monty_lib.c                  # Library API implementation
├── exec.c                       # Opcode dispatcher
├── monty_operators.c            # Arithmetic operations
├── monty_operators2.c           # Push, pall, pint operations
├── monty_operators3.c           # Pop, swap, nop operations
├── monty_operators4.c           # Additional operators
├── node_addition.c              # Stack/queue node insertion
├── queue.c                      # Queue mode operations
├── pchar.c                      # Character output
├── p_str.c                      # String output
├── rotl.c                       # Left rotation
├── rotr.c                       # Right rotation
├── freeStack.c                  # Memory deallocation
├── example_library_usage.c      # Library usage example
└── bf/                          # Brainfuck implementations
    ├── 1000-school.bf
    ├── 1001-add.bf
    ├── 1002-mul.bf
    ├── 1003-mul.bf
    └── remainder.bf
```

## Data Structures

### stack_t - Node Structure

```c
typedef struct stack_s
{
    int n;                          /* Integer value */
    struct stack_s *prev;           /* Pointer to previous element */
    struct stack_s *next;           /* Pointer to next element */
} stack_t;
```

A doubly linked list node that stores an integer and maintains bidirectional pointers.

### instruction_t - Operation Structure

```c
typedef struct instruction_s
{
    char *opcode;                   /* Operation name (e.g., "push", "add") */
    void (*f)(stack_t **, unsigned int);  /* Function pointer to operation */
} instruction_t;
```

Maps opcode strings to their respective handler functions.

### bus_t - Global State Structure

```c
typedef struct bus_s
{
    char *arg;                      /* Argument for current operation */
    FILE *file;                     /* File pointer to bytecode file */
    char *content;                  /* Current line content */
    int lifi;                       /* Mode flag: 0=stack (LIFO), 1=queue (FIFO) */
} bus_t;
```

Carries program state and current execution context across all operations.

## Notes

- Integer values are 32-bit signed integers
- Stack operations are **O(1)** complexity
- Memory is dynamically allocated for each pushed value
- All memory is properly freed on exit
- Line numbers in error messages are 1-based

## Build System (Makefile)

The project includes a comprehensive Makefile for easy compilation and distribution:

### Build Targets

```bash
make               # Build release binary (default)
make binary        # Build executable only
make static-lib    # Build static library (.a)
make shared-lib    # Build shared library (.so)
make libs          # Build both libraries
make debug         # Build with debug symbols
make release       # Build optimized version
make profile       # Build with profiling support
make clean         # Remove object files
make fclean        # Remove all build artifacts
make re            # Clean rebuild
make help          # Display help message
```

### Installation and Distribution

```bash
make install       # Install libraries to /usr/local/
make uninstall     # Remove installed files
make dist          # Create distribution tarball
make test          # Run test suite
make example       # Run example
```

### Build Output

- **Binary**: `build/bin/monty`
- **Static Library**: `build/lib/libmonty.a`
- **Shared Library**: `build/lib/libmonty.so`
- **Object Files**: `build/obj/`

## Library Support

The Monty interpreter can be compiled as a library (`libmonty`) and integrated into other C programs.

### Quick Start with Library

```bash
# Build libraries
make libs

# Install system-wide (optional)
sudo make install

# Use in your program
#include "monty_public.h"

monty_state_t *state = monty_state_init();
monty_push(state, 42);
monty_push(state, 8);
monty_add(state);
monty_state_free(state);
```

### Library Features

- ✅ **Stack/Queue Operations**: Push, pop, peek
- ✅ **Arithmetic**: Add, subtract, multiply, divide, modulo
- ✅ **Mode Switching**: Dynamic LIFO/FIFO switching
- ✅ **Error Handling**: Rich error messages
- ✅ **Multiple States**: Independent state objects
- ✅ **Clean API**: Well-documented public interface

### Sharing the Library

The library can be easily shared with others:

```bash
# Create distribution package
make dist

# Share the tarball
tar tzf dist/monty-1.0.0.tar.gz

# Receiver: Extract and install
tar xzf monty-1.0.0.tar.gz
cd monty-1.0.0
make libs
sudo make install
```

See [LIBRARY.md](LIBRARY.md) for complete library documentation.

## Robustness Features

The codebase includes several robustness improvements:

### Error Handling
- **Null pointer checks** on all inputs
- **Memory allocation validation** before use
- **Buffer overflow prevention** with proper bounds checking
- **Division by zero detection** with graceful failure
- **Stack underflow detection** with meaningful errors

### Memory Safety
- ✅ **No memory leaks** - all allocations freed properly
- ✅ **Proper deallocation order** - dependent resources freed first
- ✅ **Double-free protection** - checks before freeing
- ✅ **Uninitialized memory prevention** - all fields initialized

### File Handling
- ✅ **File existence validation** before opening
- ✅ **Proper file closure** on all exit paths
- ✅ **Read error handling** with line-by-line verification

### API Safety
- ✅ **Type checking** - validates input types
- ✅ **Range checking** - ensures values are within bounds
- ✅ **State validation** - checks preconditions before operations
- ✅ **Error propagation** - failures handled at all levels

### Library API
- ✅ **Opaque state** - implementation details hidden
- ✅ **Return codes** - consistent error reporting
- ✅ **Error messages** - detailed problem descriptions
- ✅ **Resource cleanup** - single cleanup function

## Compiler Flags

The project uses strict compiler flags for safety:

- `-Wall` - Enable common warnings
- `-Werror` - Treat warnings as errors
- `-Wextra` - Enable additional warnings
- `-pedantic` - Strict C standard compliance
- `-fPIC` - Position-independent code for libraries
- `-O2` - Optimizations (release builds)
- `-g` - Debug symbols (debug builds)

## Limitations

- No support for floating-point arithmetic
- String length limited by stack size for `pstr`
- Cannot directly access arbitrary stack positions
- Comments must start at line beginning or after whitespace
- Single-threaded (create separate states for concurrent use)

## License

This project is part of the ALX Software Engineering curriculum.

## Contributing

For improvements or bug fixes, please ensure:
1. Code follows the existing style
2. All compiler warnings are fixed
3. Memory is properly managed
4. Error handling is implemented
5. Tests pass with `make test`

## Support

For issues or questions:
- Check [LIBRARY.md](LIBRARY.md) for library usage
- Review examples in `example_library_usage.c`
- Run `make help` for build options
- Examine bytecode examples in `test/` directory

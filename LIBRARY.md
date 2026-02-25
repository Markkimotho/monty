# Using Monty as a Library

This guide explains how to use the Monty Stack/Queue library in your own C programs.

## Installation

### Option 1: System-wide Installation

```bash
# Build the libraries
make libs

# Install to system directories (/usr/local)
sudo make install

# Verify installation
ldconfig
ls -la /usr/local/lib/libmonty*
ls -la /usr/local/include/monty/
```

### Option 2: Local Library Usage

```bash
# Build libraries without system installation
make libs

# The libraries will be in:
# - build/lib/libmonty.a (static library)
# - build/lib/libmonty.so (shared library)
```

## Including the Library in Your Project

### Header Installation

```c
#include "monty_public.h"
```

## Linking Your Program

### Using System Installation

```bash
# With shared library (recommended)
gcc -o myprogram myprogram.c -lmonty

# With static library
gcc -o myprogram myprogram.c -L/usr/local/lib -lmonty -Wl,-rpath,/usr/local/lib
```

### Using Local Build

```bash
# With shared library
gcc -o myprogram myprogram.c -L./build/lib -lmonty -Wl,-rpath,./build/lib

# With static library
gcc -o myprogram myprogram.c -L./build/lib -lmonty
```

## API Reference

### State Management

```c
/* Initialize a new monty state */
monty_state_t *state = monty_state_init();

/* Free state and all its data */
monty_state_free(state);

/* Clear stack without freeing state */
monty_state_clear(state);

/* Get error message from last failed operation */
const char *err = monty_get_error(state);
```

### Stack Operations

```c
/* Push integer onto stack */
int result = monty_push(state, 42);

/* Pop from stack */
int value;
if (monty_pop(state, &value) == 0) {
    printf("Popped: %d\n", value);
}

/* Peek at top without removing */
int top;
if (monty_peek(state, &top) == 0) {
    printf("Top: %d\n", top);
}

/* Get number of elements */
int size = monty_size(state);

/* Check if empty */
if (monty_is_empty(state)) {
    printf("Stack is empty\n");
}
```

### Arithmetic Operations

```c
/* All return 0 on success, -1 on error */
monty_add(state);   /* Pop 2, push sum */
monty_sub(state);   /* Pop 2, push difference (second - top) */
monty_mul(state);   /* Pop 2, push product */
monty_div(state);   /* Pop 2, push quotient (second / top) */
monty_mod(state);   /* Pop 2, push remainder (second % top) */
```

### Mode Control

```c
/* Switch to stack (LIFO) mode - default */
monty_set_mode_stack(state);

/* Switch to queue (FIFO) mode */
monty_set_mode_queue(state);

/* Get current mode (0 = stack, 1 = queue) */
int mode = monty_get_mode(state);
```

### Debugging

```c
/* Print all stack elements to stdout */
monty_print_stack(state);
```

## Data Structures

### monty_state_t

```c
typedef struct {
    stack_t *stack;       /* The actual stack/queue data */
    int mode;             /* 0 = LIFO (stack), 1 = FIFO (queue) */
    int error_code;       /* Last error code */
    char *error_msg;      /* Last error message */
} monty_state_t;
```

### stack_t

```c
typedef struct stack_s {
    int n;                /* Integer value */
    struct stack_s *prev; /* Previous element */
    struct stack_s *next; /* Next element */
} stack_t;
```

## Error Handling

All functions that can fail return appropriate values:

- **Functions returning int**: Return 0 on success, -1 on error
- **Functions returning pointer**: Return pointer on success, NULL on error
- **Void functions**: Check error state afterwards with `monty_get_error()`

### Error Checking Pattern

```c
if (monty_pop(state, &value) == -1) {
    fprintf(stderr, "Error: %s\n", monty_get_error(state));
    /* Handle error */
}
```

## Examples

### Example 1: Simple Calculator

```c
#include "monty_public.h"
#include <stdio.h>

int main(void) {
    monty_state_t *state = monty_state_init();
    
    /* Calculate: (10 + 5) * 2 = 30 */
    monty_push(state, 10);
    monty_push(state, 5);
    monty_add(state);
    monty_push(state, 2);
    monty_mul(state);
    
    int result;
    monty_pop(state, &result);
    printf("Result: %d\n", result);
    
    monty_state_free(state);
    return 0;
}
```

### Example 2: Implementing a Queue with Mode Switch

```c
#include "monty_public.h"

int main(void) {
    monty_state_t *state = monty_state_init();
    
    /* Use as queue (FIFO) */
    monty_set_mode_queue(state);
    monty_push(state, 1);
    monty_push(state, 2);
    monty_push(state, 3);
    
    int value;
    while (!monty_is_empty(state)) {
        monty_pop(state, &value);
        printf("%d ", value);  /* Prints: 1 2 3 */
    }
    
    monty_state_free(state);
    return 0;
}
```

### Example 3: Error Recovery

```c
#include "monty_public.h"

int main(void) {
    monty_state_t *state = monty_state_init();
    
    /* Safely handle division by zero */
    monty_push(state, 10);
    monty_push(state, 0);
    
    if (monty_div(state) == -1) {
        printf("Division failed: %s\n", monty_get_error(state));
        /* Clear error state and continue */
        monty_state_clear(state);
    }
    
    monty_state_free(state);
    return 0;
}
```

## Threading and Multiple States

The library supports multiple independent states:

```c
#include "monty_public.h"

int main(void) {
    /* Create two independent states */
    monty_state_t *state1 = monty_state_init();
    monty_state_t *state2 = monty_state_init();
    
    /* Use them independently */
    monty_push(state1, 10);
    monty_push(state2, 20);
    
    printf("State 1 size: %d\n", monty_size(state1));  /* 1 */
    printf("State 2 size: %d\n", monty_size(state2));  /* 1 */
    
    monty_state_free(state1);
    monty_state_free(state2);
    return 0;
}
```

## Performance Notes

- **Push/Pop**: O(1) time complexity
- **Size calculation**: O(n) - counts elements each call
- **Memory**: Each integer takes ~24-32 bytes (depending on architecture)
- **Stack switching**: Instantaneous (just changes a flag)

## Limitations

- Integer values are 32-bit signed integers (int)
- No floating-point arithmetic
- Not thread-safe (use separate states for threads)
- Stack implemented as doubly-linked list (cache-unfriendly)

## Building Custom Applications

See `example_library_usage.c` for a complete working example with all operations demonstrated.

```bash
# Compile the example
gcc -o example example_library_usage.c -L./build/lib -lmonty \
    -Wl,-rpath,./build/lib

# Run it
./example
```

## Troubleshooting

### Library Not Found

```bash
# Make sure library is built
make libs

# Check library location
ls -la build/lib/libmonty*

# Use full path when linking
gcc -o myprogram myprogram.c -L/full/path/to/build/lib -lmonty
```

### Symbol Not Found

```c
/* Wrong: internal function */
f_add(&head, line_no);  /* Don't use internal functions */

/* Correct: public API */
monty_add(state);
```

### Memory Issues

Always pair `monty_state_init()` with `monty_state_free()`:

```c
monty_state_t *state = monty_state_init();
if (!state) {
    fprintf(stderr, "Allocation failed\n");
    return 1;
}

/* Use state... */

monty_state_free(state);  /* Always free */
```

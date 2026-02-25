/**
 * monty_public.h - Public API for Monty Stack/Queue Library
 * 
 * This header provides the public interface for using the Monty
 * stack/queue library in your own C programs.
 * 
 * Author: Monty Project
 * Version: 1.0.0
 */

#ifndef MONTY_PUBLIC_H
#define MONTY_PUBLIC_H

#include <stdio.h>
#include <stdlib.h>
#include "monty.h"  /* Include to get the monty_stack_t definition */

/* monty_stack_t is defined in monty.h - reuse that definition */

/**
 * struct monty_state - Interpreter state for library use
 * @stack: Pointer to the stack/queue (doubly linked list)
 * @mode: 0 for stack (LIFO), 1 for queue (FIFO)
 * @error_code: Last error code (0 = no error)
 * @error_msg: Last error message
 */
typedef struct monty_state_s
{
	monty_stack_t *stack;
	int mode;
	int error_code;
	char *error_msg;
} monty_state_t;

/* =========================================================================
 * STACK OPERATIONS
 * ===================================================================== */

/**
 * monty_push - Push integer value onto stack/queue
 * @state: Monty state structure
 * @value: Integer to push
 * 
 * Return: 0 on success, -1 on error
 */
int monty_push(monty_state_t *state, int value);

/**
 * monty_pop - Remove and return top element
 * @state: Monty state structure
 * @value: Pointer to store popped value
 * 
 * Return: 0 on success, -1 on error (empty stack)
 */
int monty_pop(monty_state_t *state, int *value);

/**
 * monty_peek - Get top element without removing
 * @state: Monty state structure
 * @value: Pointer to store peeked value
 * 
 * Return: 0 on success, -1 on error (empty stack)
 */
int monty_peek(monty_state_t *state, int *value);

/**
 * monty_size - Get size of stack
 * @state: Monty state structure
 * 
 * Return: Number of elements in stack
 */
int monty_size(const monty_state_t *state);

/**
 * monty_is_empty - Check if stack is empty
 * @state: Monty state structure
 * 
 * Return: 1 if empty, 0 otherwise
 */
int monty_is_empty(const monty_state_t *state);

/* =========================================================================
 * ARITHMETIC OPERATIONS
 * ===================================================================== */

/**
 * monty_add - Add top two elements
 * @state: Monty state structure
 * 
 * Return: 0 on success, -1 on error
 */
int monty_add(monty_state_t *state);

/**
 * monty_sub - Subtract top from second (second - top)
 * @state: Monty state structure
 * 
 * Return: 0 on success, -1 on error
 */
int monty_sub(monty_state_t *state);

/**
 * monty_mul - Multiply top two elements
 * @state: Monty state structure
 * 
 * Return: 0 on success, -1 on error
 */
int monty_mul(monty_state_t *state);

/**
 * monty_div - Divide second by top (second / top)
 * @state: Monty state structure
 * 
 * Return: 0 on success, -1 on error (div by zero or stack too short)
 */
int monty_div(monty_state_t *state);

/**
 * monty_mod - Modulo second by top (second % top)
 * @state: Monty state structure
 * 
 * Return: 0 on success, -1 on error (div by zero or stack too short)
 */
int monty_mod(monty_state_t *state);

/* =========================================================================
 * MODE OPERATIONS
 * ===================================================================== */

/**
 * monty_set_mode_stack - Switch to stack (LIFO) mode
 * @state: Monty state structure
 */
void monty_set_mode_stack(monty_state_t *state);

/**
 * monty_set_mode_queue - Switch to queue (FIFO) mode
 * @state: Monty state structure
 */
void monty_set_mode_queue(monty_state_t *state);

/**
 * monty_get_mode - Get current mode
 * @state: Monty state structure
 * 
 * Return: 0 for stack, 1 for queue
 */
int monty_get_mode(const monty_state_t *state);

/* =========================================================================
 * STATE MANAGEMENT
 * ===================================================================== */

/**
 * monty_state_init - Initialize new monty state
 * 
 * Return: Pointer to new state, NULL on error
 */
monty_state_t *monty_state_init(void);

/**
 * monty_state_free - Free monty state and its stack
 * @state: Monty state structure to free
 */
void monty_state_free(monty_state_t *state);

/**
 * monty_state_clear - Clear stack without freeing state
 * @state: Monty state structure
 */
void monty_state_clear(monty_state_t *state);

/**
 * monty_get_error - Get last error message
 * @state: Monty state structure
 * 
 * Return: Error message string
 */
const char *monty_get_error(const monty_state_t *state);

/**
 * monty_print_stack - Print all stack elements (for debugging)
 * @state: Monty state structure
 */
void monty_print_stack(const monty_state_t *state);

#endif /* MONTY_PUBLIC_H */

/**
 * monty_lib.c - Public API implementation for Monty library
 * 
 * This file implements the public interface for using Monty as a library
 * in other C programs. It wraps the internal functionality with a
 * cleaner, more portable interface.
 */

#include "monty.h"
#include "monty_public.h"
#include <string.h>

/* Global bus structure used by internal functions */
/* This is initialized for library use */
bus_t bus = {NULL, NULL, NULL, 0};

/**
 * monty_state_init - Initialize new monty state
 */
monty_state_t *monty_state_init(void)
{
	monty_state_t *state;

	state = malloc(sizeof(monty_state_t));
	if (!state)
		return (NULL);
	
	state->stack = NULL;
	state->mode = 0;  /* Stack mode (LIFO) by default */
	state->error_code = 0;
	state->error_msg = NULL;
	
	return (state);
}

/**
 * monty_state_free - Free monty state and its stack
 */
void monty_state_free(monty_state_t *state)
{
	if (!state)
		return;
	
	if (state->stack)
		free_stack(state->stack);
	
	if (state->error_msg)
		free(state->error_msg);
	
	free(state);
}

/**
 * monty_state_clear - Clear stack without freeing state
 */
void monty_state_clear(monty_state_t *state)
{
	if (!state)
		return;
	
	if (state->stack)
	{
		free_stack(state->stack);
		state->stack = NULL;
	}
	
	if (state->error_msg)
	{
		free(state->error_msg);
		state->error_msg = NULL;
	}
	
	state->error_code = 0;
}

/**
 * monty_set_error - Set error message in state
 */
static void monty_set_error(monty_state_t *state, int code, const char *msg)
{
	state->error_code = code;
	if (state->error_msg)
		free(state->error_msg);
	if (msg)
	{
		state->error_msg = malloc(strlen(msg) + 1);
		if (state->error_msg)
			strcpy(state->error_msg, msg);
	}
	else
		state->error_msg = NULL;
}

/**
 * monty_push - Push integer value onto stack/queue
 */
int monty_push(monty_state_t *state, int value)
{
	monty_stack_t *new_node;

	if (!state)
	{
		return (-1);
	}

	new_node = malloc(sizeof(monty_stack_t));
	if (!new_node)
	{
		monty_set_error(state, -1, "Memory allocation failed");
		return (-1);
	}

	new_node->n = value;
	new_node->prev = NULL;
	new_node->next = NULL;

	if (state->mode == 0)  /* Stack mode (LIFO) */
	{
		if (state->stack)
			state->stack->prev = new_node;
		new_node->next = state->stack;
		state->stack = new_node;
	}
	else  /* Queue mode (FIFO) */
	{
		if (!state->stack)
		{
			state->stack = new_node;
		}
		else
		{
			monty_stack_t *temp = state->stack;
			while (temp->next)
				temp = temp->next;
			temp->next = new_node;
			new_node->prev = temp;
		}
	}

	return (0);
}

/**
 * monty_pop - Remove and return top element
 */
int monty_pop(monty_state_t *state, int *value)
{
	monty_stack_t *temp;

	if (!state || !value)
		return (-1);

	if (!state->stack)
	{
		monty_set_error(state, -1, "Can't pop from empty stack");
		return (-1);
	}

	*value = state->stack->n;
	temp = state->stack;
	state->stack = state->stack->next;
	if (state->stack)
		state->stack->prev = NULL;
	free(temp);

	return (0);
}

/**
 * monty_peek - Get top element without removing
 */
int monty_peek(monty_state_t *state, int *value)
{
	if (!state || !value)
		return (-1);

	if (!state->stack)
	{
		monty_set_error(state, -1, "Can't peek at empty stack");
		return (-1);
	}

	*value = state->stack->n;
	return (0);
}

/**
 * monty_size - Get size of stack
 */
int monty_size(const monty_state_t *state)
{
	int count = 0;
	monty_stack_t *temp;

	if (!state)
		return (0);

	temp = state->stack;
	while (temp)
	{
		count++;
		temp = temp->next;
	}

	return (count);
}

/**
 * monty_is_empty - Check if stack is empty
 */
int monty_is_empty(const monty_state_t *state)
{
	if (!state)
		return (1);

	return (state->stack == NULL);
}

/**
 * monty_add - Add top two elements
 */
int monty_add(monty_state_t *state)
{
	int val1, val2, result;

	if (!state)
		return (-1);

	if (monty_size(state) < 2)
	{
		monty_set_error(state, -1, "Stack too short for add operation");
		return (-1);
	}

	if (monty_pop(state, &val1) == -1)
		return (-1);
	if (monty_pop(state, &val2) == -1)
	{
		monty_push(state, val1);  /* Restore state */
		return (-1);
	}

	result = val1 + val2;
	return (monty_push(state, result));
}

/**
 * monty_sub - Subtract top from second (second - top)
 */
int monty_sub(monty_state_t *state)
{
	int val1, val2, result;

	if (!state)
		return (-1);

	if (monty_size(state) < 2)
	{
		monty_set_error(state, -1, "Stack too short for sub operation");
		return (-1);
	}

	if (monty_pop(state, &val1) == -1)
		return (-1);
	if (monty_pop(state, &val2) == -1)
	{
		monty_push(state, val1);
		return (-1);
	}

	result = val2 - val1;
	return (monty_push(state, result));
}

/**
 * monty_mul - Multiply top two elements
 */
int monty_mul(monty_state_t *state)
{
	int val1, val2, result;

	if (!state)
		return (-1);

	if (monty_size(state) < 2)
	{
		monty_set_error(state, -1, "Stack too short for mul operation");
		return (-1);
	}

	if (monty_pop(state, &val1) == -1)
		return (-1);
	if (monty_pop(state, &val2) == -1)
	{
		monty_push(state, val1);
		return (-1);
	}

	result = val1 * val2;
	return (monty_push(state, result));
}

/**
 * monty_div - Divide second by top (second / top)
 */
int monty_div(monty_state_t *state)
{
	int val1, val2, result;

	if (!state)
		return (-1);

	if (monty_size(state) < 2)
	{
		monty_set_error(state, -1, "Stack too short for div operation");
		return (-1);
	}

	if (monty_pop(state, &val1) == -1)
		return (-1);

	if (val1 == 0)
	{
		monty_push(state, val1);  /* Restore */
		monty_set_error(state, -1, "Division by zero");
		return (-1);
	}

	if (monty_pop(state, &val2) == -1)
	{
		monty_push(state, val1);
		return (-1);
	}

	result = val2 / val1;
	return (monty_push(state, result));
}

/**
 * monty_mod - Modulo second by top (second % top)
 */
int monty_mod(monty_state_t *state)
{
	int val1, val2, result;

	if (!state)
		return (-1);

	if (monty_size(state) < 2)
	{
		monty_set_error(state, -1, "Stack too short for mod operation");
		return (-1);
	}

	if (monty_pop(state, &val1) == -1)
		return (-1);

	if (val1 == 0)
	{
		monty_push(state, val1);
		monty_set_error(state, -1, "Division by zero");
		return (-1);
	}

	if (monty_pop(state, &val2) == -1)
	{
		monty_push(state, val1);
		return (-1);
	}

	result = val2 % val1;
	return (monty_push(state, result));
}

/**
 * monty_set_mode_stack - Switch to stack (LIFO) mode
 */
void monty_set_mode_stack(monty_state_t *state)
{
	if (state)
		state->mode = 0;
}

/**
 * monty_set_mode_queue - Switch to queue (FIFO) mode
 */
void monty_set_mode_queue(monty_state_t *state)
{
	if (state)
		state->mode = 1;
}

/**
 * monty_get_mode - Get current mode
 */
int monty_get_mode(const monty_state_t *state)
{
	if (!state)
		return (-1);

	return (state->mode);
}

/**
 * monty_get_error - Get last error message
 */
const char *monty_get_error(const monty_state_t *state)
{
	if (!state || !state->error_msg)
		return ("No error");

	return (state->error_msg);
}

/**
 * monty_print_stack - Print all stack elements (for debugging)
 */
void monty_print_stack(const monty_state_t *state)
{
	monty_stack_t *temp;

	if (!state)
		return;

	printf("=== Stack Contents ===\n");
	if (!state->stack)
	{
		printf("[empty]\n");
		return;
	}

	temp = state->stack;
	while (temp)
	{
		printf("%d\n", temp->n);
		temp = temp->next;
	}
	printf("=== End of Stack ===\n");
}

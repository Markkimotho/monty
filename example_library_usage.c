/**
 * example_library_usage.c - Example of using Monty as a library
 * 
 * This demonstrates how to integrate the Monty library into your own programs.
 * Compile with: gcc -o example example_library_usage.c -L. -lmonty -lm
 * Or if using the header as a system library: gcc -o example example_library_usage.c -lmonty
 */

#include "monty_public.h"
#include <stdio.h>

void print_section(const char *title)
{
	printf("\n=== %s ===\n", title);
}

int main(void)
{
	monty_state_t *state;
	int value;

	/* Initialize the state */
	state = monty_state_init();
	if (!state)
	{
		fprintf(stderr, "Failed to initialize Monty state\n");
		return (1);
	}

	/* Example 1: Basic stack operations */
	print_section("Example 1: Push and Pop");
	printf("Pushing 10, 20, 30...\n");
	monty_push(state, 10);
	monty_push(state, 20);
	monty_push(state, 30);
	
	printf("Current stack size: %d\n", monty_size(state));
	printf("Stack contents:\n");
	monty_print_stack(state);

	/* Example 2: Peeking without removing */
	print_section("Example 2: Peek");
	if (monty_peek(state, &value) == 0)
		printf("Top element (peek): %d\n", value);
	printf("Stack size after peek: %d\n", monty_size(state));

	/* Example 3: Pop elements */
	print_section("Example 3: Pop");
	if (monty_pop(state, &value) == 0)
		printf("Popped value: %d\n", value);
	if (monty_pop(state, &value) == 0)
		printf("Popped value: %d\n", value);
	printf("Stack size after pops: %d\n", monty_size(state));

	/* Example 4: Arithmetic operations */
	print_section("Example 4: Arithmetic Operations");
	monty_state_clear(state);
	printf("Pushing 15 and 7...\n");
	monty_push(state, 15);
	monty_push(state, 7);
	
	printf("Before addition: ");
	monty_print_stack(state);
	
	if (monty_add(state) == 0)
	{
		printf("After addition (15 + 7): ");
		monty_print_stack(state);
	}

	/* Example 5: Subtraction */
	print_section("Example 5: Subtraction");
	monty_state_clear(state);
	printf("Pushing 20 and 5...\n");
	monty_push(state, 20);
	monty_push(state, 5);
	
	if (monty_sub(state) == 0)
	{
		printf("After subtraction (20 - 5): ");
		monty_print_stack(state);
	}

	/* Example 6: Multiplication */
	print_section("Example 6: Multiplication");
	monty_state_clear(state);
	printf("Pushing 6 and 7...\n");
	monty_push(state, 6);
	monty_push(state, 7);
	
	if (monty_mul(state) == 0)
	{
		printf("After multiplication (6 * 7): ");
		monty_print_stack(state);
	}

	/* Example 7: Division */
	print_section("Example 7: Division");
	monty_state_clear(state);
	printf("Pushing 50 and 5...\n");
	monty_push(state, 50);
	monty_push(state, 5);
	
	if (monty_div(state) == 0)
	{
		printf("After division (50 / 5): ");
		monty_print_stack(state);
	}

	/* Example 8: Modulo */
	print_section("Example 8: Modulo");
	monty_state_clear(state);
	printf("Pushing 17 and 5...\n");
	monty_push(state, 17);
	monty_push(state, 5);
	
	if (monty_mod(state) == 0)
	{
		printf("After modulo (17 %% 5): ");
		monty_print_stack(state);
	}

	/* Example 9: Mode switching - Queue mode */
	print_section("Example 9: Queue Mode (FIFO)");
	monty_state_clear(state);
	printf("Switching to queue mode...\n");
	monty_set_mode_queue(state);
	
	printf("Pushing 1, 2, 3 in queue mode (FIFO)...\n");
	monty_push(state, 1);
	monty_push(state, 2);
	monty_push(state, 3);
	
	printf("Queue (FIFO order): ");
	monty_print_stack(state);

	/* Example 10: Error handling */
	print_section("Example 10: Error Handling");
	monty_state_clear(state);
	printf("Attempting to pop from empty stack...\n");
	if (monty_pop(state, &value) == -1)
		printf("Error: %s\n", monty_get_error(state));

	printf("Attempting division by zero...\n");
	monty_push(state, 10);
	monty_push(state, 0);
	if (monty_div(state) == -1)
		printf("Error: %s\n", monty_get_error(state));

	/* Example 11: Complex calculation */
	print_section("Example 11: Complex Calculation");
	monty_state_clear(state);
	monty_set_mode_stack(state);
	printf("Calculating: ((10 + 5) * 2)\n");
	
	monty_push(state, 10);
	monty_push(state, 5);
	monty_add(state);    /* 10 + 5 = 15 */
	monty_push(state, 2);
	monty_mul(state);    /* 15 * 2 = 30 */
	
	printf("Result: ");
	monty_print_stack(state);

	/* Example 12: Check if empty */
	print_section("Example 12: Check Empty Stack");
	monty_state_clear(state);
	printf("Stack empty? %s\n", monty_is_empty(state) ? "Yes" : "No");
	monty_push(state, 42);
	printf("After pushing 42: %s\n", monty_is_empty(state) ? "Yes" : "No");

	/* Cleanup */
	print_section("Cleanup");
	monty_state_free(state);
	printf("State freed successfully\n");

	return (0);
}

#ifndef TADEM_H
#define TADEM_H

/* Define feature test macros before system headers to avoid conflicts */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

/**
* struct monty_stack_s - doubly linked list representation of a stack (or queue)
* @n: integer
* @prev: points to the previous element of the stack (or queue)
* @next: points to the next element of the stack (or queue)
*
* Description: doubly linked list node structure
* for stack, queues, LIFO, FIFO
*/
typedef struct monty_stack_s
{
	int n;
	struct monty_stack_s *prev;
	struct monty_stack_s *next;
} monty_stack_t;


/**
* struct instruction_s - opcode and its function
* @opcode: the opcode
* @f: function to handle the opcode
*
* Description: opcode and its function
* for stack, queues, LIFO, FIFO
*/
typedef struct instruction_s
{
	char *opcode;
	void (*f)(monty_stack_t **stack, unsigned int line_number);
} instruction_t;


/**
* struct bus_s - variables -args, file, line content
* @arg: value
* @file: pointer to monty file
* @content: line content
* @lifi: flag change stack <-> queue
*
* Description: carries values through the program
*/
typedef struct bus_s
{
	char *arg;
	FILE *file;
	char *content;
	int lifi;
}  bus_t;
extern bus_t bus;

char *_realloc(char *ptr, unsigned int old_size, unsigned int new_size);
ssize_t getstdin(char **lineptr, int file);
char  *clean_line(char *content);
void f_push(monty_stack_t **head, unsigned int number);
void f_pall(monty_stack_t **head, unsigned int number);
void f_pint(monty_stack_t **head, unsigned int number);
int execute(char *content, monty_stack_t **head, unsigned int counter, FILE *file);
void free_stack(monty_stack_t *head);
void f_pop(monty_stack_t **head, unsigned int counter);
void f_swap(monty_stack_t **head, unsigned int counter);
void f_add(monty_stack_t **head, unsigned int counter);
void f_nop(monty_stack_t **head, unsigned int counter);
void f_sub(monty_stack_t **head, unsigned int counter);
void f_div(monty_stack_t **head, unsigned int counter);
void f_mul(monty_stack_t **head, unsigned int counter);
void f_mod(monty_stack_t **head, unsigned int counter);
void f_pchar(monty_stack_t **head, unsigned int counter);
void f_pstr(monty_stack_t **head, unsigned int counter);
void f_rotl(monty_stack_t **head, unsigned int counter);
void f_rotr(monty_stack_t **head, __attribute__((unused)) unsigned int counter);
void addnode(monty_stack_t **head, int n);
void addqueue(monty_stack_t **head, int n);
void f_queue(monty_stack_t **head, unsigned int counter);
void f_stack(monty_stack_t **head, unsigned int counter);


#endif

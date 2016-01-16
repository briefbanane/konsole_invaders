/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		16.08.2011
 * Last change:	15.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

#include "input.h"
#include "terminal.h"
#include "output.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
 * A concatenate List is used to manage the users inputs.
 */
typedef struct _input* input;

/*
 * (char)		character:	A single character of the input.
 * (input)		next:	Pointer to the next input.
 */
struct _input
{
	char character;
	input next;
};

/*
 * This function is the thread itself. The terminal will be prepared, the input will be read and
 * the old state of the terminal will be restored. This goes on, until end is set.
 * The list of the input will be deleted at the end of the thread.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void input_thread(void);

/*
 * (input)	read:	The pointer to the last, not read input.
 */
input read = NULL;
/*
 * (input)	write:	The pointer to the last input.
 */
input write = NULL;
/*
 * (int)	end:	Shows, if the input-thread should be stopped.
 */
int end = 0;
/*
 * (int)	ended:	Shows, if the input-thread has ended.
 */
int ended = 0;
/*
 * (pthread_t *)	thread:	The pointer to the input-thread-function.
 */
pthread_t *thread = NULL;

void input_thread(void)
{
	char c;
	input tmp;
	//The first node is created.
	tmp = malloc(sizeof(struct _input));
	//The input-thread is necessary for the program.
	assert(tmp != NULL);
	tmp->next = NULL;
	read = write = tmp;
	//The loop is started.
	while (!end)
	{
		//Waiting for input.
		prepare_terminal();
		c = getchar();
		recover_terminal();
		//The input will be saved in a new node.
		write->character = c;
		tmp = malloc(sizeof(struct _input));
		if (tmp == NULL)
			print_warning("Not enough memory!!", "input_thread", "input.c");
		else
		{
			tmp->next = NULL;
			write->next = tmp;
			write = write->next;
		}
	}
	//Delete the list.
	while (read != write)
	{
		tmp = read;
		read = read->next;
		free(tmp);
	}
	tmp = read;
	write = read = NULL;
	free(tmp);
	ended = 1;
}

int get_next_input(void)
{
	char c;
	input tmp;
	if (read == write)
		return NO_NEW_INPUT;
	//Read the next input.
	c = read->character;
	//Delete the first node.
	tmp = read;
	read = read->next;
	free(tmp);
	return c;
}

void start_input_thread(void)
{
	if (thread == NULL)
	{
		thread = malloc(sizeof(pthread_t));
		end = ended = 0;
		//Start thread.
		assert(thread != NULL && pthread_create(thread, NULL, (void *)input_thread, NULL) == 0);
	}
	else
		print_warning("Thread already created!!", "start_input_thread", "input.c");
}

void stop_input_thread(void)
{
	if (thread != NULL)
	{
		end = 1;
		//Read a last input, because getchar() blocks the thread.
		print_text("Hit any key to quit:", field_height + 2, 0);
		while (!ended)
			system("sleep 0.05");
		free(thread);
		thread = NULL;
	}
	else
		print_warning("No thread to stop!!", "stop_input_thread", "input.c");
}

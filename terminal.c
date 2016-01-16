/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		16.08.2011
 * Last change:	22.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */


#include "terminal.h"
#include "output.h"
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/*
 * Saves the old terminal state.
 */
struct termios *terminal_old = NULL;

void prepare_terminal(void)
{
	struct termios *terminal = malloc(sizeof(struct termios));
	terminal_old = malloc(sizeof(struct termios));
	//Is there enough memory?
	assert(terminal != NULL);
	//No echo and no cannonical mode (see manpage termios).
	terminal->c_lflag &= ~(ECHO|ICANON);
	terminal->c_cc[VMIN] = 1;
	terminal->c_cc[VTIME] = 0;
	//Saves the new attributes.
	assert(terminal_old != NULL && tcgetattr(STDIN_FILENO, terminal_old) == 0);
	//Set new attributes.
	tcsetattr(STDIN_FILENO, TCSANOW, terminal);
	free(terminal);
}

void recover_terminal(void)
{
	if (terminal_old != NULL)
	{
		//Set old attributes.
		if (tcsetattr(STDIN_FILENO, TCSANOW, terminal_old) == -1)
			print_warning("Couldn't recover terminal attributes!!", "recover_terminal", "terminal.c");
		free(terminal_old);
		terminal_old = NULL;
	}
}
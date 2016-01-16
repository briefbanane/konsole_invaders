/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		16.08.2011
 * Last change:	16.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

/*
 * Scope:	This module makes it possible to read the user-input while the other parts of the programs are running.
 * 			The thread is started with start_input_thread and stopped with stop_input_thread.
 * 			If the thread is running the input can be read with get_next_input.
 */

#ifndef __INPUT_H__
#define __INPUT_H__

/*
 * Shows that there is no new input.
 */
#define NO_NEW_INPUT -1

/*
 * This funciton returns the last input character of the user or NO_NEW_INPUT if there is no new input.
 * Before this function is called start_input_thread has to be called.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (int)	The last input of the user or NO_NEW_INPUT.
 */
int get_next_input(void);

/*
 * Starts the input-thread if it isn't running yet.
 * Now the inputs can be read with get_next_input.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void start_input_thread(void);

/*
 * Stops the input-thread. All inputs will be deleted.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void stop_input_thread(void);

#endif
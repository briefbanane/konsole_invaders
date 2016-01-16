/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		16.08.2011
 * Last change:	22.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

/*
 * Scope:	You can prepare the terminal to disable echo and read the user-input immediatly.
 * 			Then you can recover the terminal to its old state.
 */

#ifndef __TERMINAL_H__
#define __TERMINAL_H__

/*
 * Prepares the terminal to the user-input. The echo will be disabled.
 * If this isn't possible the program terminates with an assert().
 * After reading the user-input you should call recover_terminal.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void prepare_terminal(void);

/*
 * Recovers the old state of the terminal.
 * The funciton prepare_terminal must be called before.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void recover_terminal(void);

#endif
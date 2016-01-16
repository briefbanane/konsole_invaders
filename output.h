/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		17.08.2011
 * Last change:	21.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

/*
 * Scope:	In this module are defined various output-functions.
 * 			But the terminal must understand escape-sequences.
 * 			print_warning can be used to show warning outside the field.
 * 			print_border prints a border around the field.
 * 			set_colors sets the colors of the new text.
 * 			clear_screen clears the screen.
 * 			print_text can be used to print strings at arbitrary positions in the terminal.
 */

#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <assert.h>

/*
 * The escape-character has this ASCII-code.
 * Escape-sequences are used to set the cursorposition and the colors.
 * Also the arrow keys are represented throgh escape-sequences.
 */
#define ESC 0x1B
/*
 * The second character of an escape-sequence.
 */
#define SEQUENCE_FOLLOWER '['

/*
 * These constants are the parameters of set_colors.
 * Each constant represents a color.
 */
enum
{
	BLACK = 0,
	RED,
	GREEN,
	ORANGE,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
};

/*
 * Shows the handed over message below the field.
 * <<<INPUT>>>
 * (char *)	message:	The description of the warning.
 * (char *)	function:	The name of the function where the error happened.
 * (char *)	file:		The name of the file where the error happened.
 * <<<OUTPUT>>>
 * (void)
 */
void print_warning(const char *message, const char *function, const char *file);

/*
 * Sets the colors of the next output.
 * <<<INPUT>>>
 * (int)	foreground:	The new color of the foreground.
 * (int)	background:	The new color of the background.
 * <<<OUTPUT>>>
 * (void)
 */
void set_colors(int foreground, int background);

/*
 * Clears the screen.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void clear_screen(void);

/*
 * Prints a border around the field of #-characters.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void print_border(void);

/*
 * Prints the text at the specified position in the terminal.
 * row=0 means, that the text is printed in the first row after the border.
 * With column it's the same.
 * <<<INPUT>>>
 * (char *)	text:	The text, which will be printed.
 * (int)	row:	The row where you want to print the text. Must be greater than 0.
 * (int)	column:	The column where you want to print the text. Must be greater than 0.
 * <<<OUTPUT>>>
 * (void)
 */
void print_text(const char *text, int row, int column);

#endif
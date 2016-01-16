/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		17.08.2011
 * Last change:	21.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

#include "output.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * This constant plus the value of the color (see output.h) is needed in the escape-sequence to set the colors of the text.
 */
#define FOREGROUND 30
/*
 * This constant plus the value of the color (see output.h) is needed in the escape-sequence to set the colors of the text.
 */
#define BACKGROUND 40

/*
 * The escape-sequence starts to count at 1 and the border is 1, so the coordinates must be displaced of this constant.
 */
#define DISTANCE 2

void print_warning(const char *message, const char *function, const char *file)
{
	//A Buffer.
	char *string = malloc(sizeof(char) * 200);
	int count;
	if (string != NULL && message != NULL && function != NULL && file != NULL)
	{
		//The warning will be showed below the field.
		count = snprintf(string, 199, "WARNING: %s in funkcion <<%s>> in file <<%s>>\n", message, function, file);
		string[count + 1] = '\0';
		set_colors(WHITE, BLACK);
		print_text(string, field_height + 2, 0);
	}
	if (string != NULL)
		free(string);
}

void set_colors(int foreground, int background)
{
	int check = 1;
	if (foreground < BLACK || foreground > WHITE || background < BLACK || foreground > WHITE)
		print_warning("Illegal Colors!!", "set_colors", "output.c");
	else
		//This escape-sequence sets the colors of the text.
		check = (fprintf(stdout, "%c[1;%i;%im", ESC, FOREGROUND + foreground, BACKGROUND + background) > 0);
	fflush(stdout);
	assert(check);
}

void clear_screen(void)
{
	assert(system("clear") == EXIT_SUCCESS);
}

void print_border(void)
{
	int i, check = 1;
	set_colors(WHITE, BLACK);
	for (i = 1; i <= field_length + DISTANCE && check; i++)
	{
		//This escape-sequence sets the position of the cursor (without the #).
		check = (fprintf(stdout, "%c[%i;%iH#", ESC, 1, i) > 0);
		check = check && (fprintf(stdout, "%c[%i;%iH#", ESC, field_height + DISTANCE, i) > 0);
	}
	for (i = DISTANCE; i < field_height + DISTANCE && check; i++)
	{
		//This escape-sequence sets the position of the cursor (without the #).
		check = (fprintf(stdout, "%c[%i;%iH#", ESC, i, 1) > 0);
		check = check && (fprintf(stdout, "%c[%i;%iH#", ESC, i, field_length + DISTANCE) > 0);
	}
	fflush(stdout);
	assert(check);
}

void print_text(const char *text, int row, int column)
{
	int check, i;
	if (text != NULL && row >= 0 && column >= 0)
	{
		//This escape-sequence sets the position of the cursor.
		check = (fprintf(stdout, "%c[%i;%iH", ESC, row + DISTANCE, column + DISTANCE) > 0);
		if (row > field_height)
			for (i = 0; i < 2 *field_length; i++)
				check = check && (fprintf(stdout, " ") > 0);
		//This escape-sequence sets the position of the cursor (without the string).
		check = check && (fprintf(stdout, "%c[%i;%iH%s", ESC, row + DISTANCE, column + DISTANCE, text) > 0);
		fflush(stdout);
		assert(check);
	}
}
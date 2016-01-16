/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		22.09.2011
 * Last change:	22.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

#include "output.h"
#include "input.h"
#include "types.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Sets the constants of the parameter-menu.
 */
enum
{
	CONFIG_GAME = 0,
	CONFIG_HEIGHT,
	CONFIG_LENGTH,
	CONFIG_WAVES,
	CONFIG_ENEMY_COLUMNS,
	CONFIG_LIFE,
	CONFIG_END,
	CONFIG_NUMBER_OF_FIELDS
};

/*
 * It will appear a menu, where the user can set certain parameters of the game.
 * The configuration will be saved in the file ".konsole_invaders.conf",
 * to be present for the next game start.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void options(void);
/*
 * The Height of the field.
 */
int field_height = 40;
/*
 * The Length of the field.
 */
int field_length = 100;

/*
 * The number of enemies per line.
 */
int enemy_columns = 10;
/*
 * The number of waves.
 */
int number_of_waves = 15;
/*
 * If it is 1, the player gets a life and a rocket more every second wave.
 */
int life = 1;
/*
 * The file-pointer that points to the configuration-file.
 */
FILE *config = NULL;
/*
 * Sets the minimum values of the paramters in the parameter-menu.
 * The index must be decreased of 1, because CONFIG_GAME has no limits.
 */
int minima[] = {20, 4, 1, 1, 0};
/*
 * Sets the maximum values of the paramters in the parameter-menu.
 * The index must be decreased of 1, because CONFIG_GAME has no limits.
 */
int maxima[] = {MAX_HEIGHT, MAX_LENGTH, 15, ENEMY_COLUMNS_MAX, 1};
/*
 * Saves the parameters of the game.
 */
int values[CONFIG_END + 1];

void read_configuration(void)
{
	int input, input2;
	//Save values in the array.
	values[CONFIG_HEIGHT] = field_height;
	values[CONFIG_LENGTH] = field_length;
	values[CONFIG_WAVES] = number_of_waves;
	values[CONFIG_ENEMY_COLUMNS] = enemy_columns;
	values[CONFIG_LIFE] = life;
	//Create file if nonexistent and open it for reading.
	if ((config = fopen(".konsole_invaders.conf", "a+")) != NULL)
	{
		fclose(config);
		if ((config = fopen(".konsole_invaders.conf", "r")) != NULL)
		{
			//Read game parameters an set them if they are inside the limits.
			while (fscanf(config, "%i %i", &input, &input2) != EOF)
				if (input2 >= minima[input - 1] && input2 <= maxima[input - 1])
					values[input] = input2;
			if (values[CONFIG_ENEMY_COLUMNS] * 4 > values[CONFIG_LENGTH])
				values[CONFIG_ENEMY_COLUMNS] = values[CONFIG_LENGTH] / 4;
			fclose(config);
		}
	}
	//Rewrite the (maybe new) values.
	field_height = values[CONFIG_HEIGHT];
	field_length = values[CONFIG_LENGTH];
	number_of_waves = values[CONFIG_WAVES];
	enemy_columns = values[CONFIG_ENEMY_COLUMNS];
	life = values[CONFIG_LIFE];
}

void menu(void)
{
	enum
	{
		GAME = 0,
		OPTIONS,
		END,
		NUMBER_OF_FIELDS
	};
	char *chooseable_fields[NUMBER_OF_FIELDS] = {"Play", "Options", "Quit"};
	int esc = 0, esc_sequence = 0, input, chosen = GAME, i;
	do
	{
		//The chosen field is printed in color, the others in white.
		for (i = 0; i < NUMBER_OF_FIELDS; i++)
		{
			set_colors(WHITE, BLACK);
			if (i == chosen)
				set_colors(GREEN, BLACK);
			print_text(chooseable_fields[i], field_height / 2 - NUMBER_OF_FIELDS / 2 + i, field_length / 4);
		}
		//With space the field will be chosen.
		while ((input = get_next_input()) != NO_NEW_INPUT && input != ' ')
		{
			//If escape is pressed two times the program terminates.
			if (esc && input == ESC)
				exit(EXIT_FAILURE);
			//Go one field up.
			if ((esc_sequence && input == 'A') || (esc_sequence && input == 'a') ||
				(!esc_sequence && input == 'w') || (!esc_sequence && input == 'k'))
				chosen = (chosen - 1 + NUMBER_OF_FIELDS) % NUMBER_OF_FIELDS;
			//Go one field down.
			if ((esc_sequence && input == 'B') || (esc_sequence && input == 'b') ||
				(!esc_sequence && input == 's') || (!esc_sequence && input == 'j'))
				chosen = (chosen + 1) % NUMBER_OF_FIELDS;
			//Is there a special sequence?
			esc_sequence = (input == SEQUENCE_FOLLOWER && esc);
			esc = (input == ESC);
		}
		system("sleep 0.05");
	}
	while (input != ' ');
	switch (chosen)
	{
		case GAME:
			return;
		case END:
			exit(EXIT_SUCCESS);
		case OPTIONS:
			options();
	}
}

void options(void)
{
	char *chooseable_fields[CONFIG_NUMBER_OF_FIELDS] = {"Play", "Height", "Length", "Waves", "Enemy-columns", "More Life", "Quit"};
	char *text = malloc(sizeof(char) * 100);
	int esc = 0, esc_sequence = 0, input, chosen = CONFIG_GAME, i, count;
	do
	{
		for (i = 0; i < CONFIG_NUMBER_OF_FIELDS; i++)
		{
			//The chosen field will be printed in color. Also the value of the parameter will be printed behind.
			set_colors(WHITE, BLACK);
			if (i == chosen)
				set_colors(GREEN, BLACK);
			if (i > CONFIG_GAME && i < CONFIG_END)
			{
				count = snprintf(text, 99, "%s", chooseable_fields[i]);
				count = snprintf(text + count, 100 - count, " %i  ", values[i]);
				print_text(text, field_height / 2 - CONFIG_NUMBER_OF_FIELDS / 2 + i, field_length / 4);
			}
			else
				print_text(chooseable_fields[i], field_height / 2 - CONFIG_NUMBER_OF_FIELDS / 2 + i, field_length / 4);
		}
		while ((input = get_next_input()) != NO_NEW_INPUT && input != ' ')
		{
			//If escape is pressed two times the program terminates.
			if (esc && input == ESC)
				exit(EXIT_FAILURE);
			//Go one field up.
			if ((esc_sequence && input == 'A') || (esc_sequence && input == 'a') ||
				(!esc_sequence && input == 'w') || (!esc_sequence && input == 'k'))
				chosen = (chosen - 1 + CONFIG_NUMBER_OF_FIELDS) % CONFIG_NUMBER_OF_FIELDS;
			//Increase paramter value.
			if ((esc_sequence && input == 'C') || (esc_sequence && input == 'c') ||
				(!esc_sequence && input == 'd') || (!esc_sequence && input == 'l'))
				if (values[chosen] + 1 <= maxima[chosen - 1])
					values[chosen]++;
			//Decrease paramter value.
			if ((esc_sequence && input == 'D') || (esc_sequence && input == 'd') ||
				(!esc_sequence && input == 'a') || (!esc_sequence && input == 'h'))
				if (values[chosen] - 1 >= minima[chosen - 1])
					values[chosen]--;
			//Go one field up.
			if ((esc_sequence && input == 'B') || (esc_sequence && input == 'b') ||
				(!esc_sequence && input == 's') || (!esc_sequence && input == 'j'))
				chosen = (chosen + 1) % CONFIG_NUMBER_OF_FIELDS;
			//Is there a special sequence?
				esc_sequence = (input == SEQUENCE_FOLLOWER && esc);
				esc = (input == ESC);
		}
		system("sleep 0.05");
	}
	while (input != ' ');
	//Write the new values into the configuration-file.
	if ((config = fopen(".konsole_invaders.conf", "w")) != NULL)
		for (i = CONFIG_GAME + 1; i < CONFIG_END; i++)
			fprintf(config, "%i %i\n", i, values[i]);
	//Set new values.
	field_height = values[CONFIG_HEIGHT];
	field_length = values[CONFIG_LENGTH];
	number_of_waves = values[CONFIG_WAVES];
	enemy_columns = values[CONFIG_ENEMY_COLUMNS];
	life = values[CONFIG_LIFE];
	switch (chosen)
	{
		case CONFIG_GAME:
			return;
		case CONFIG_END:
			exit(EXIT_SUCCESS);
	}
}

/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		17.08.2011
 * Last change:	8.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

/*
 * Scope:	Connects the single parts of the program so, that it is like the game Space Invaders™(c).
 */

#include "output.h"
#include "terminal.h"
#include "input.h"
#include "shot.h"
#include "ship.h"
#include "types.h"
#include "config.h"
#include "interaction.h"
#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Writes Integers into the Array enemytypes which represent the different kinds of enemies.
 * The higher the parameter number is, the stronger will the enemies be.
 * The maximum number is 15.
 * <<<INPUT>>>
 * (int)	number:	The number of the wave.
 * <<<OUTPUT>>>
 * (void)
 */
void make_wave(int number);

/*
 * This function should be registered with atexit(). It makes different kinds of cleaning (free reserved memory etc.).
 * Also the input-thread will be stopped and the old state of the terminal will be restored.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void clean(void);

/*
 * Saves the Ship of the player.
 */
ship player = NULL;
/*
 * Saves the enemies of the current wave.
 */
ship **enemies = NULL;
/*
 * Saves the single types of the enemies of the current wave.
 * With make_wave(wavenumber) the array will be filled.
 */
int enemytypes[ENEMY_ROWS][ENEMY_COLUMNS_MAX];
/*
 * In this String informations to the game will be saved (points, number of rockets, etc.).
 */
char *info = NULL;

void make_wave(int number)
{
	int i, j, linetypes[ENEMY_ROWS];
	if (number <= 0 || number > 15)
		print_warning("Illegal wavenumber!!", "make_wave", "main.c");
	else
	{
		//Cleaning the array.
		for (i = 0; i < ENEMY_ROWS; i++)
			linetypes[i] = 0;
		//The strength of the enemies will be increased by each wavenumber.
		for (i = 0; number > 0; i++, number--)
		{
			while (linetypes[i % ENEMY_ROWS] == 3)
				i++;
			linetypes[i % ENEMY_ROWS]++;
			//After increasing of a linetype the following linetypes will be set to 0.
			for (j = i % ENEMY_ROWS + 1; j < ENEMY_ROWS; j++)
				linetypes[j] = 0;
		}
	}
	//The type of each line will be written in each column of the line.
	for (i = 0; i < ENEMY_ROWS; i++)
		for (j = 0; j < ENEMY_COLUMNS_MAX; j++)
			enemytypes[i][j] = linetypes[i];
}

void clean(void)
{
	int i;
	void *tmp = NULL;
	delete_all_enemies(enemies);
	if (enemies != NULL)
	{
		for (i = 0; i < ENEMY_ROWS; i++)
			if (enemies[i] != NULL)
			{
				tmp = enemies[i];
				enemies[i] = NULL;
				free(tmp);
			}
		tmp = enemies;
		enemies = NULL;
		free(tmp);
	}
	if (info != NULL)
	{
		tmp = info;
		info = NULL;
		free(tmp);
	}
	if (player != NULL)
		delete_ship_memory(&player);
	delete_all_shots();
	stop_input_thread();
	clear_screen();
	recover_terminal();
}

int main(void)
{
	int result = GAME_WAVE_BEATEN, wave = 0, count, rockets;
	char *info = malloc(sizeof(char) * 100);
	read_configuration();
	//Preparing the Screen.
	clear_screen();
	print_border();
	//Inform user about GPL.
	print_text("Konsole Invaders Copyright (C) 2011 Fabian Ebner", field_height / 2 - 1, field_length / 4);
	print_text("This program comes with ABSOLUTELY NO WARRANTY.", field_height / 2, field_length / 4);
	print_text("This is free software, and you are welcome to redistribute it", field_height / 2 + 1, field_length / 4);
	print_text("under certain conditions. For details see the file LICENSE", field_height / 2 + 2, field_length / 4);
	print_text("Press Return to continue: ", field_height / 2 + 3, field_length / 4);
	getchar();
	//Preparing the Screen.
	clear_screen();
	print_border();
	//Create input-thread.
	start_input_thread();
	atexit((void *)clean);
	menu();
	//Preparing the Screen.
	clear_screen();
	print_border();
	//Create the ship of the player.
	player = create_ship_player();
	assert(player != NULL);
	do
	{
		if (info != NULL)
		{
			//Print some relevant information for the game.
			rockets = remaining_shots_player();
			count = snprintf(info, 99, "wave nr. %i of %i    points: %i    lifes: %i    rockets: %i", wave, number_of_waves, points_player(), health_points(player), rockets);
			info[count + 1] = '\0';
			set_colors(WHITE, BLACK);
			print_text(info, field_height + 1, 0);
		}
		if (result == GAME_WAVE_BEATEN)
		{
			//Create next wave or quit the game.
			delete_all_shots();
			if (wave >= number_of_waves)
				break;
			increase_points_player(wave * 100);
			make_wave(++wave);
			//Each second wave the player gets a life and a shot more if life is set.
			if (wave % 2 == 0 && life)
			{
				increase_player_shot_number();
				increase_ship_life(player);
			}
			enemies = create_enemy_ships(enemytypes);
			assert(enemies != NULL);
			result = GAME_NOTHING;
		}
		//Preparing for the register_object()-calls in the move-functions.
		init_register();
		//Move the mobile objects.
		if (result == GAME_NOTHING)
			result = move_ship_player(&player);
		if (result == GAME_NOTHING)
			result = move_enemy_ships(enemies);
		if (result == GAME_NOTHING)
			move_shots();
		system("sleep 0.05");
	}
	while (result != GAME_LOST);
	if (result == GAME_LOST)
	{
		//The player has lost.
		set_colors(WHITE, BLACK);
		print_text("You've lost!!!", field_height + 1, 0);
		system("sleep 3");
		if (info != NULL)
		{
			count = snprintf(info, 99, "You have reached wave number %i and made %i points!!!", wave, points_player());
			info[count + 1] = '\0';
			set_colors(WHITE, BLACK);
			print_text(info, field_height + 1, 0);
		}
	}
	else
	{
		//The player has won.
		set_colors(WHITE, BLACK);
		print_text("You have won!!! You've defeated all waves!!!!!", field_height + 1, 0);
		system("sleep 3");
		if (info != NULL)
		{
			count = snprintf(info, 99, "You've defeated all %i waves and made %i points!!!", number_of_waves, points_player());
			info[count + 1] = '\0';
			set_colors(WHITE, BLACK);
			print_text(info, field_height + 1, 0);
		}
	}
	system("sleep 5");
	exit(EXIT_SUCCESS);
}

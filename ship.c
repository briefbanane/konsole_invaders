/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		30.08.2011
 * Last change:	21.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

#include "ship.h"
#include "output.h"
#include "input.h"
#include "shot.h"
#include "interaction.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/*
 * This constant is the speed in left x-direction of the enemies.
 */
#define SHIP_LEFT 1
/*
 * This constant is the speed in right x-direction of the enemies.
 */
#define SHIP_RIGHT -1
/*
 * This constant shows if the enemies should move down.
 */
#define SHIP_DOWN 2

/*
 * The shot-rate of the player has no value.
 */
#define SHOT_RATE_PLAYER 0

/*
 * This structure simulates a simple spaceship.
 * (char *[])	print:				How the ship looks like.
 * (int)		x:					The x-coordinate of the position of the ship.
 * (int)		y:					The y-coordinate of the position of the ship.
 * (int)		health_points:		The remaining life of the ship.
 * (int)		health_points_max:	The maximum life of the ship.
 * (int)		shot_rate:			The higher the shot_rate the more frequent the ship will fire.
 */
struct spaceship
{
	char *print[3];
	int x;
	int y;
	int health_points;
	int health_points_max;
	int shot_rate;
};

/*
 * Creates a enemy-ship of the type type.
 * The higher the type the stronger the ship is.
 * <<<INPUT>>>
 * (int)	type:	The type of the ship. A 0 means no ship; other types are 1, 2 and 3.
 * (int)	x:		The x-coordinate of the ship.
 * (int)	y:		The y-coordinate of the ship.
 * <<<OUTPUT>>>
 * (ship)	The new enemy-ship.
 */
ship create_enemy_ship(int type, int x, int y);

/*
 * The ship will blink 3 times.
 * <<<INPUT>>>
 * (ship)	s:	The ship that should blink.
 * <<<OUTPUT>>>
 * (void)
 */
void blink(ship s);

/*
 * Determines the new moving-direction of the enemies and sets it.
 * <<<INPUT>>>
 * (ship **)	s:	The enemies those moving-direction will be determinated.
 * <<<OUTPUT>>>
 * (void)
 */
void get_direction(ship **s);

/*
 * Moves the ship along the actually set direction. Also the shots will be created.
 * <<<INPUT>>>
 * (ship *)	s:	The adress of the ship that should be moved.
 * <<<OUTPUT>>>
 * (void)
 */
void move_enemy_ship(ship *s);

/*
 * Decreases the life of the ship and frees the memory if it was the last healthpoint of the ship.
 * If it is the ship of the player it will blink 3 times.
 * <<<INPUT>>>
 * (ship *)	s:	The adress of the ship that should be damaged.
 * <<<OUTPUT>>>
 * (void)
 */
void hit_ship(ship *s);

/*
 * Prints the ship at its actual position.
 * <<<INPUT>>>
 * (ship)	s:	The ship that should be printed.
 * <<<OUTPUT>>>
 * (void)
 */
void print_ship(ship s);

/*
 * Deletes the ship from the screen.
 * <<<INPUT>>>
 * (ship)	s:	The ship to delete.
 * <<<OUTPUT>>>
 * (void)
 */
void delete_ship(ship s);

/*
 * A pointer to blink_thread if it is running else a nullpointer.
 */
pthread_t *blink_thread = NULL;
/*
 * The actual moving-direction of the enemies. Possible values are SHIP_DOWN, SHIP_RIGHT and SHIP_LEFT.
 */
int enemies_moving_direction = SHIP_LEFT;
/*
 * The last moving-direction of the enemies. Only SHIP_LEFT and SHIP_RIGHT are possible.
 */
int enemies_moving_direction_old = SHIP_LEFT;

ship create_enemy_ship(int type, int x, int y)
{
	ship s = malloc(sizeof(struct spaceship));
	if (s == NULL)
		print_warning("Not enough memory!!", "create_enemy_ship", "ship.c");
	else
	{
		//Create ship from type.
		switch (type)
		{
			case 0:
				free(s);
				return NULL;
			case 1:
				s->print[0] = "/-\\";
				s->print[1] = "|*|";
				s->print[2] = "u u";
				s->health_points = s->health_points_max = 5;
				s->shot_rate = 5;
				break;
			case 2:
				s->print[0] = "/M\\";
				s->print[1] = "*+*";
				s->print[2] = "W W";
				s->health_points = s->health_points_max = 10;
				s->shot_rate = 10;
				break;
			case 3:
				s->print[0] = "/-\\";
				s->print[1] = "\\+/";
				s->print[2] = " V ";
				s->health_points = s->health_points_max = 20;
				s->shot_rate = 10;
				break;
			default:
				print_warning("Illegal ship-type!!", "create_enemy_ship", "ship.c");
		}
		//Set the position.
		s->x = x;
		s->y = y;
	}
	return s;
}

void blink(ship s)
{
	int i;
	pthread_t *tmp = blink_thread;
	for (i = 0; i < 3; i++)
	{
		//With the sleep-system-calls the blinking will be visible.
		delete_ship(s);
		system("sleep 0.1");
		print_ship(s);
		system("sleep 0.1");
		
	}
	delete_ship(s);
	blink_thread = NULL;
	free(tmp);
}

void get_direction(ship **s)
{
	int i, j, change = 0, new_x;
	if (s != NULL)
	{
		for (i = 0; i < ENEMY_ROWS && !change; i++)
			if (s[i] != NULL)
				for (j = 0; j < enemy_columns && !change; j++)
					if (s[i][j] != NULL)
					{
						//If the border is reached by a ship the ships will be moved down.
						new_x = s[i][j]-> x + enemies_moving_direction;
						change = (new_x < 0 || new_x > field_length - 3);
					}
		if (change)
			enemies_moving_direction = SHIP_DOWN;
	}
}

void move_enemy_ship(ship *s)
{
	int i;
	if (s == NULL || *s == NULL)
	{
		print_warning("Function was called with nullpointer!!", "move_enemy_ship", "ship.c");
		return;
	}
	delete_ship(*s);
	//The ship will be moved with the global moving-direction.
	if (enemies_moving_direction == SHIP_DOWN)
		(*s)->y++;
	else
		(*s)->x += enemies_moving_direction;
	//With a certain probability the ship will fire a shot.
	if (rand() % 1000 < (*s)->shot_rate * (1 + (1 - (1.0 * (*s)->health_points) / (1.0 * (*s)->health_points_max))))
		create_shot(ENEMY_SHOT, (*s)->x + 1, (*s)->y + 2);
	print_ship(*s);
	for (i = 0; i < 9 && s != NULL && *s != NULL; i++)
		register_object((*s)->x + i / 3, (*s)->y + i % 3, (void **)s, FIELD_ENEMY);
}

void hit_ship(ship *s)
{
	if (s != NULL && *s != NULL)
	{
		if (--(*s)->health_points == 0)
			delete_ship_memory(s);
	}
	//The ship of the player should blink if it is hit.
	if (s != NULL && *s != NULL && (*s)->shot_rate == SHOT_RATE_PLAYER)
	{
		blink_thread = malloc(sizeof(pthread_t));
		if (blink_thread == NULL || pthread_create(blink_thread, NULL, (void *)blink, (void *)(*s)) != 0)
			print_warning("Couldn't create thread!!!", "hit_ship", "ship.c");
	}
}

void print_ship(ship s)
{
	int i;
	if (s != NULL)
	{
		//The color is determinated by the remaining healthpoints.
		switch((int)(1.0 * s->health_points_max / s->health_points + 0.5))
		{
			case 1:
				set_colors(GREEN, BLACK);
				break;
			case 2:
				set_colors(ORANGE, BLACK);
				break;
			default:
				set_colors(RED, BLACK);
		}
		for (i = 0; i < 3; i++)
			print_text(s->print[i], s->y + i, s->x);
	}
}

void delete_ship(ship s)
{
	int i;
	if (s != NULL)
	{
		for (i = 0; i < 3; i++)
			print_text("   ", s->y + i, s->x);
	}
}

ship create_ship_player(void)
{
	ship s = malloc(sizeof(struct spaceship));
	if (s == NULL)
		print_warning("Not enough memory!!!", "create_ship_player", "ship.c");
	else
	{
		s->print[0] = " ^ ";
		s->print[1] = "/+\\";
		s->print[2] = "***";
		//The position is initially down at the middle.
		s->x = field_length / 2 - 1;
		s->y = field_height - 3;
		s->health_points = s->health_points_max = 3;
		s->shot_rate = SHOT_RATE_PLAYER;
	}
	return s;
}

ship **create_enemy_ships(int type[ENEMY_ROWS][ENEMY_COLUMNS_MAX])
{
	ship **s = malloc(sizeof(ship *) * ENEMY_ROWS);
	int i, j;
	//After each wave the random-generator will be reinitialised.
	srand(time(NULL));
	if (s == NULL)
		print_warning("Not enough memory!!!", "create_enemy_ships", "ship.c");
	else
	{
		for (i = 0; i < ENEMY_ROWS; i++)
		{
			s[i] = malloc(sizeof(struct spaceship) * ENEMY_COLUMNS_MAX);
			if (s[i] == NULL)
			{
				print_warning("Not enough memory!!!", "create_enemy_ships", "ship.c");
				for (j = 0; j < i; j++)
				{
					free(s[i]);
					s[i] = NULL;
				}
				free(s);
				return NULL;
			}
		}
		for (i = 0; i < ENEMY_ROWS; i++)
			for (j = 0; j < ENEMY_COLUMNS_MAX; j++)
				//The position of the ship is determinated by the position in the array.
				s[i][j] = create_enemy_ship(type[i][j], j * 4, i * 4);
	}
	return s;
}

int move_ship_player(ship *s)
{
	int esc = 0, esc_sequence = 0, input = get_next_input(), clear = (input != NO_NEW_INPUT), i;
	if (s == NULL || *s == NULL)
		return GAME_LOST;
	//Only if the ship will be moved it will be deleted.
	if (clear)
		delete_ship(*s);
	do
	{
		//If escape is pressed two times the program terminates.
		if (esc && input == ESC)
			exit(EXIT_FAILURE);
		//Go one field right (d, l, arrow right)
		if ((esc_sequence && input == 'c') || (esc_sequence && input == 'C') || (!esc_sequence && input == 'l') || (!esc_sequence && input == 'd'))
			if ((*s)->x + 1 <= field_length - 3)
				(*s)->x++;
		//Go one field left(a, h, arrow left)
		if ((esc_sequence && input == 'd') || (esc_sequence && input == 'D') || (!esc_sequence && input == 'h') || (!esc_sequence && input == 'a'))
			if ((*s)->x - 1 >= 0)
				(*s)->x--;
		//Shoot (space, arrow up, w, k)
		if ((!esc_sequence && input == ' ') || (esc_sequence && input == 'A') || (esc_sequence && input == 'a') ||
			(!esc_sequence && input == 'w') || (!esc_sequence && input == 'k'))
			create_shot(PLAYER_SHOT, (*s)->x + 1, (*s)->y - 2);
		//Is there a special sequence.
		esc_sequence = (input == SEQUENCE_FOLLOWER && esc);
		esc = (input == ESC);
	}
	while ((input = get_next_input()) != NO_NEW_INPUT);
	for (i = 0; i < 9 && s != NULL && *s != NULL; i++)
		register_object((*s)->x + i / 3, (*s)->y + i % 3, (void **)s, FIELD_PLAYER);
	if (s == NULL && *s == NULL)
		return GAME_LOST;
	//If the ship is blinking don t draw it.
	if (blink_thread == NULL)
		print_ship(*s);
	return GAME_NOTHING;
}

int move_enemy_ships(ship **s)
{
	int i, j, reset = 0, enemycount = 0;
	if (s == NULL)
		return GAME_WAVE_BEATEN;
	get_direction(s);
	for (i = 0; i < ENEMY_ROWS && !reset; i++)
		if (s[i] != NULL)
			for (j = 0; j < enemy_columns && !reset; j++)
				if (s[i][j] != NULL)
				{
					enemycount++;
					if (s[i][j]->y < field_height - 3)
						move_enemy_ship(&(s[i][j]));
					else
						reset = 1;
				}
	//After moving the ships down invert the horizontal moving-direction.
	if (enemies_moving_direction == SHIP_DOWN)
		enemies_moving_direction = enemies_moving_direction_old = -enemies_moving_direction_old;
	//If the enemies reach the lowest row, reset them.
	if (reset)
		for (i = 0; i < ENEMY_ROWS; i++)
			if (s[i] != NULL)
				for (j = 0; j < enemy_columns; j++)
					if (s[i][j] != NULL)
					{
						delete_ship(s[i][j]);
						s[i][j]->x = j * 4;
						s[i][j]->y = i * 4;
					}
	if (!enemycount)
		return GAME_WAVE_BEATEN;
	return GAME_NOTHING;
}

void increase_ship_life(ship s)
{
	if (s != NULL)
	{
		s->health_points++;
		s->health_points_max++;
	}
}

int health_points(ship s)
{
	if (s == NULL)
		return 0;
	return s->health_points;
}

void delete_ship_memory(ship *s)
{
	ship tmp = *s;
	if (s != NULL && *s != NULL)
	{
		delete_ship(*s);
		*s = NULL;
		free(tmp);
	}
}

void delete_all_enemies(ship **s)
{
	int i, j;
	if (s != NULL)
	{
		for (i = 0; i < ENEMY_ROWS; i++)
			if (s[i] != NULL)
				for (j = 0; j < ENEMY_COLUMNS_MAX; j++)
					if (s[i][j] != NULL)
					{
						delete_ship(s[i][j]);
						free(s[i][j]);
						s[i][j] = NULL;
					}
	}
}

void collision_ship(ship *s)
{
	hit_ship(s);
}

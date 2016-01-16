/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		30.08.2011
 * Last change:	22.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

#include "shot.h"
#include "output.h"
#include "interaction.h"
#include "config.h"
#include <stdlib.h>

/*
 * The maximum number of shots that exist at the same time.
 */
#define MAX_SHOT 10000

/*
 * Simulates a simple shot.
 * (int)	direction:	ENEMY_SHOT or PLAYER_SHOT.
 * (int)	x:			The x-coordinate of the shot.
 * (int)	y:			The y-coordinate of the shot.
 */
struct ship_shot
{
	int direction;
	int x;
	int y;
};

/*
 * Prints the shot on the screen.
 * <<<INPUT>>>
 * (shot)	s:	The shot that should be printed.
 * <<<OUTPUT>>>
 * (void)
 */
void print_shot(shot s);

/*
 * Deletes the shot from the screen.
 * <<<INPUT>>>
 * (shot)	s:	The shot to delete.
 * <<<OUTPUT>>>
 * (void)
 */
void delete_shot(shot s);

/*
 * Moves the shot into his direction. If the shot reaches the end of the field it will be deleted.
 * <<<INPUT>>>
 * (shot *)	s:	The adress of the shot to move.
 * <<<OUTPUT>>>
 * (void)
 */
void move_shot(shot *s);

/*
 * The shot will be deleted from memory and screen.
 * <<<INPUT>>>
 * (shot *)	s:	The adress of the shot to remove.
 * <<<OUTPUT>>>
 * (void)
 */
void delete_shot_memory(shot *s);

/*
 * Counts the number of shots fired by the player that exist.
 */
int player_shot_count = 0;
/*
 * The maximum number of shots that can be fired by the player.
 */
int max_player_shot = 5;
/*
 * The array that saves the shots. Because it is global it is filled with nullpointers.
 */
shot shots[MAX_SHOT];
/*
 * Saves the maximum index where a shot in the array is.
 */
int maximum_index = 0;

void print_shot(shot s)
{
	if (s != NULL)
	{
		//The colors and the symbol are in relation of the type of the shot.
		if (s->direction == PLAYER_SHOT)
		{
			set_colors(BLUE, BLACK);
			print_text("+", s->y, s->x);
			print_text(" ", s->y + 1, s->x);
		}
		else
		{
			set_colors(RED, BLACK);
			print_text("*", s->y, s->x);
			print_text("V", s->y + 1, s->x);
		}
	}
}

void delete_shot(shot s)
{
	if (s != NULL)
	{
		print_text(" ", s->y, s->x);
		print_text(" ", s->y + 1, s->x);
	}
}

void move_shot(shot *s)
{
	int new_y;
	if (s != NULL && *s != NULL)
	{
		new_y = (*s)->y + (*s)->direction;
		if (new_y >= 0 && new_y < field_height - 1)
		{
			delete_shot(*s);
			(*s)->y = new_y;
			print_shot(*s);
			//Register all fields of the shot.
			if ((*s)->direction == PLAYER_SHOT)
			{
				register_object((*s)->x, (*s)->y, (void **)s, FIELD_SHOT_PLAYER);
				if (s != NULL && *s != NULL)
					register_object((*s)->x, (*s)->y + 1, (void **)s, FIELD_SHOT_PLAYER);
			}
			else
			{
				register_object((*s)->x, (*s)->y, (void **)s, FIELD_SHOT_ENEMY);
				if (s != NULL && *s != NULL)
					register_object((*s)->x, (*s)->y + 1, (void **)s, FIELD_SHOT_ENEMY);
			}
		}
		else
		{
			//If the shot reaches the end of the field it will be deleted.
			delete_shot(*s);
			delete_shot_memory(s);
		}
	}
}

void delete_shot_memory(shot *s)
{
	shot tmp = *s;
	if (s != NULL && *s != NULL)
	{
		delete_shot(*s);
		if ((*s)->direction == PLAYER_SHOT)
			player_shot_count--;
		*s = NULL;
		free(tmp);
	}
}

void create_shot(int direction, int x, int y)
{
	shot s = malloc(sizeof(struct ship_shot));
	int i;
	if (s == NULL)
		print_warning("Not enough memory!!!", "create_shot", "shot.c");
	else
		if (x >= 0 && x < field_length && y >= 0 && y < field_height - 1)
		{
			//Can the player shoot again?
			if (player_shot_count < max_player_shot || direction != PLAYER_SHOT)
			{
				i = 0;
				//Search a free place in the array.
				while (i < maximum_index && shots[i] != NULL)
					i++;
				if (i < MAX_SHOT)
				{
					if (direction == PLAYER_SHOT)
						player_shot_count++;
					//Set values.
					s->direction = direction;
					s->x = x;
					s->y = y;
					shots[i] = s;
					//Is the shot behind the maximum index?
					if (i + 1 > maximum_index)
						maximum_index = i + 1;
				}
			}
		}
}

void move_shots(void)
{
	int i, j = 0;
	for (i = 0; i < maximum_index; i++)
		if (shots[i] != NULL)
			move_shot(&shots[i]);
	/*
	 * Fill the empty positions at the array.
	 */
	for (i = maximum_index - 1; i >= 0; i--)
		if (shots[i] == NULL)
			continue;
		else
		{
			while (j < i && shots[j] != NULL)
				j++;
			if (j < i)
			{
				shots[j] = shots[i];
				shots[i] = NULL;
			}
		}
	//Determine new maximum index.
	for (i = maximum_index; i >= 0 && shots[i] == NULL; i--);
		maximum_index = i + 1;
}

int remaining_shots_player(void)
{
	return max_player_shot - player_shot_count;
}

void increase_player_shot_number(void)
{
	max_player_shot++;
}

void delete_all_shots(void)
{
	int i;
	for (i = 0; i < maximum_index; i++)
		if (shots[i] != NULL)
			delete_shot_memory(&shots[i]);
	maximum_index = 0;
}

void collision_shot(shot *s)
{
	delete_shot_memory(s);
}
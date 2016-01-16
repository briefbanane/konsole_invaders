/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		30.08.2011
 * Last change:	13.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

#include "interaction.h"
#include "shot.h"
#include "ship.h"
#include "output.h"
#include "types.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Should be called when two objects are at the same position in the field.
 * The collison-functions of the objects will be called when necessary.
 * If both objects are equal shots one collision_shot will be called, if the shots are different both collision_shot will be called.
 * If both objects are ships both collision_ship will be called.
 * If one object is a ship and the other one a shot of the other ship-type collision_shot and collision_ship will be called.
 * <<<INPUT>>>
 * (void **)	a:		The adress of the first object.
 * (int)		type_a:	The type of the first object.
 * (void **)	b:		The adress of the second object.
 * (int)		type_b:	The type of the second object.
 * <<<OUTPUT>>>
 * (void)
 */
void collision(void **a, int type_a, void **b, int type_b);

/*
 * For each adress in objects will be checked if its a pointer to a NULL-pointer.
 * If it is the pointer in objects is set to a NULL-pointer and objecttypes will be set to FIELD_NOTHING at that position.
 * This function should be called after collisions.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void check_pointer(void);

/*
 * Saves the points of the player.
 */
int points = 0;
/*
 * In this array the adresses of the struct-pointer will be saved when registering.
 * It is necessary, because deleting a struct-pointer will now be visible everywhere in the code.
 */
void **objects[MAX_HEIGHT][MAX_LENGTH];
/*
 * In this array the types of the obects will be saved when registering.
 * The types are defined in interacion.h.
 */
int objecttypes[MAX_HEIGHT][MAX_LENGTH];

void collision(void **a, int type_a, void **b, int type_b)
{
	/*
	 * The points will be increased of 5 if an enemy-ship gets hit and of 3 if a enemy-shot is destroyed.
	 */
	switch (type_a)
	{
		case FIELD_PLAYER:
			switch (type_b)
			{
				case FIELD_SHOT_ENEMY:
					collision_ship((ship *)a);
					collision_shot((shot *)b);
					break;
				case FIELD_ENEMY:
					collision_ship((ship *)a);
					collision_ship((ship *)b);
					increase_points_player(5);
					break;
			}
			break;
		case FIELD_ENEMY:
			switch (type_b)
			{
				case FIELD_SHOT_PLAYER:
					collision_ship((ship *)a);
					collision_shot((shot *)b);
					increase_points_player(3);
					break;
				case FIELD_PLAYER:
					collision_ship((ship *)a);
					collision_ship((ship *)b);
					increase_points_player(5);
					break;
			}
			break;
		case FIELD_SHOT_PLAYER:
			switch (type_b)
			{
				case FIELD_SHOT_ENEMY:
					collision_shot((shot *)b);
					increase_points_player(3);
				case FIELD_SHOT_PLAYER:
					//If there collide two shots of the same type the last one will be deleted.
					collision_shot((shot *)a);
					break;
				case FIELD_ENEMY:
					collision_shot((shot *)a);
					collision_ship((ship *)b);
					increase_points_player(5);
					break;
			}
			break;
		case FIELD_SHOT_ENEMY:
			switch (type_b)
			{
				case FIELD_SHOT_PLAYER:
					collision_shot((shot *)b);
					increase_points_player(3);
				case FIELD_SHOT_ENEMY:
					collision_shot((shot *)a);
					//If there collide two shots of the same type the last one will be deleted.
					break;
				case FIELD_PLAYER:
					collision_shot((shot *)a);
					collision_ship((ship *)b);
					increase_points_player(3);
					break;
			}
			break;
		default:
			print_warning("Function was called with illegal type!!", "collision", "interaction.c");
	}
}

void check_pointer(void)
{
	int i, j;
	for (i = 0; i < field_height; i++)
		for (j = 0; j < field_length; j++)
			if (objects[i][j] != NULL && *objects[i][j] == NULL)
			{
				objecttypes[i][j] = FIELD_NOTHING;
				objects[i][j] = NULL;
			}
}

void init_register(void)
{
	int i, j;
	for (i = 0; i < field_height; i++)
		for (j = 0; j < field_length; j++)
		{
			objects[i][j] = NULL;
			objecttypes[i][j] = FIELD_NOTHING;
		}
}

void register_object(int x, int y, void **objectadress, int type)
{
	if (x >= 0 && x < field_length && y >= 0 && y < field_height)
	{
			if (type == FIELD_NOTHING)
			{
				objects[y][x] = NULL;
				objecttypes[y][x] = FIELD_NOTHING;
			}
			else
				if (objects[y][x] == NULL || objecttypes[y][x] == FIELD_NOTHING)
				{
					//There is not yet an object at this place.
					objects[y][x] = objectadress;
					objecttypes[y][x] = type;
				}
				else
				{
					//Collision: There is another object at the same position.
					collision(objectadress, type, objects[y][x], objecttypes[y][x]);
					check_pointer();
				}
	}
}

int points_player(void)
{
	return points;
}

void increase_points_player(int value)
{
	if (value > 0)
		points += value;
}
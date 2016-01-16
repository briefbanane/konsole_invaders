/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		30.08.2011
 * Last change:	22.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

/*
 * Scope:	This module contains the structur spaceship and makes the work with it possible.
 * 			You can create enemy-ships and player-ships and you can move them.
 * 			The shots will be created while moving, but to move the shots you have to use another function (see shot.h).
 * 			You can increase the healthpoints, hit the ship with collision_ship and delete the ships.
 */

#ifndef __SHIP_H__
#define __SHIP_H__

#include "types.h"

/*
 * Creates the ship of the player.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (ship)	The ship of the player.
 */
ship create_ship_player(void);

/*
 * Creates an array of the types in the array type.
 * Possible types are 0, 1, 2 or 3 and a 0 means no ship.
 * The higher the type the stronger the ships.
 * <<<INPUT>>>
 * (int[][])	type:	The array that contains the types of the single ships.
 * <<<OUTPUT>>>
 * (ship **)	The created ships.
 */
ship **create_enemy_ships(int type[ENEMY_ROWS][ENEMY_COLUMNS_MAX]);

/*
 * Reads the user inputs from get_next_input in input.h and moves the ship of the player.
 * The input-thread has to be created before. Also the shots will be created.
 * Returns if the player has lost the game (when the ship is null or has no healthpoints) or not.
 * <<<INPUT>>>
 * (ship *)		s:	The adress of the ship of the player.
 * <<<OUTPUT>>>
 * (int)	GAME_LOST or GAME_NOTHING.
 */
int move_ship_player(ship *s);

/*
 * Moves all enemies in the array s. The ships will move horizontal until the end of the row and then one row down.
 * If the ships reach the ground there positions will be reset. Also the shots will be created.
 * Returns if the waves has been beaten (when s is null or all enemies are null) or not.
 * <<<INPUT>>>
 * (ship **)	s:	The array of the enemies with size [ENEMY_ROWS][ENEMY_COLUMNS_MAX].
 * <<<OUTPUT>>>
 * (int)	GAME_NOTHING or GAME_WAVE_BEATEN.
 */
int move_enemy_ships(ship **s);

/*
 * Increases the healthpoints of the ship by 1. Also the maximum healthpoints will be increased by 1.
 * <<<INPUT>>>
 * (ship)	s:	The ship that should recieve more healthpoints.
 * <<<OUTPUT>>>
 * (void)
 */
void increase_ship_life(ship s);

/*
 * Returns how many healthpoints are left.
 * If the ship is null 0 is returned.
 * <<<INPUT>>>
 * (ship)	s:	The ship which healthpoints should be returned.
 * <<<OUTPUT>>>
 * (int)	The remaining life of the ship.
 */
int health_points(ship s);

/*
 * Deletes the ship from memory and screen.
 * <<<INUPT>>>
 * (ship *)	s:	The adress of the ship that should be removed.
 * <<<OUTPUT>>>
 * (void)
 */
void delete_ship_memory(ship *s);

/*
 * Deletes all enemies in the array from memory and screen.
 * <<<INPUT>>>
 * (ship **)	s:	The array with the enemies of size [ENEMY_ROWS][ENEMY_COLUMNS_MAX].
 * <<<OUTPUT>>>
 * (void)
 */
void delete_all_enemies(ship **s);

/*
 * This function should be called when the ship collides with another object.
 * The healthpoints of the ship will be decreased by 1. If the ship has no more life it will be removed.
 * <<<INPUT>>>
 * (ship *)	s:	The adress of the ship that should be moved.
 * <<<OUTPUT>>>
 * (void)
 */
void collision_ship(ship *s);

#endif
/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		30.08.2011
 * Last change:	22.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

/*
 * Scope:	This module contains the structur ship_shot and makes the work with it possible.
 * 			You can create, move and delete shots. The memory-maintance is implemented inside the module.
 * 			With collison shot you can react to collisions with other objects.
 */

#ifndef __SHOT_H__
#define __SHOT_H__

#include "types.h"

/*
 * This constant shows, that the shot is from the player.
 * It is also the vertical speed of the shot.
 */
#define PLAYER_SHOT -1
/*
 * This constant shows, that the shot is form an enemy.
 * It is also the vertical speed of the shot.
 */
#define ENEMY_SHOT 1

/*
 * Creates a shot at the position (x,y). The coordinates have to be inside the field.
 * <<<INPUT>>>
 * (int)	direction:			PLAYER_SHOT or ENEMY_SHOT.
 * (int)	x:					The x-coordinate of the shot.
 * (int)	y:					The y-coordinate of the shot.
 * <<<OUTPUT>>>
 * (void)
 */
void create_shot(int direction, int x, int y);

/*
 * All created shots will be moved. If a shot collides or reaches the end of the field, it will be deleted.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void move_shots(void);

/*
 * Returns how many shots the player has left.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (int)	The number of shots the player has left.
 */
int remaining_shots_player(void);

/*
 * Increases the maximum number of shots of the player.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void increase_player_shot_number(void);

/*
 * Deletes all created shots from memory and screen.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void delete_all_shots(void);

/*
 * This function should be called when the shot collides with another object.
 * The shot will be deleted from memory and screen.
 * <<<INPUT>>>
 * (shot *)	s:	The adress of the shot that collides with another object.
 * <<<OUTPUT>>>
 * (void)
 */
void collision_shot(shot *s);

#endif
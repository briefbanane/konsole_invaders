/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		30.08.2011
 * Last change:	8.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

/*
 * Scope:	This module is used that there can be reacted if two objects are at the same position in the field.
 * 			Also the points of the player are managed.
 * 			With init_register all old registrations are deleted. It should be called before registering all objects.
 * 			With register a position in the field can be "reserved" for a object.
 * 			Is a second object registered at the same position the collision-functions of the two objects will be called.
 * 			With increase_points_player the points of the player can be increased.
 * 			With points_player you can read the points of the player.
 */

#ifndef __INTERACTION_H__
#define __INTERACTION_H__

/*
 * Shows, that on this position is no object.
 */
#define FIELD_NOTHING 0
/*
 * Shows, that on this position is a shot of the player.
 */
#define FIELD_SHOT_PLAYER 1
/*
 * Shows, that on this position is a shot of an enemy.
 */
#define FIELD_SHOT_ENEMY 2
/*
 * Shows, that on this position is the players ship.
 */
#define FIELD_PLAYER 3
/*
 * Shows, that on this position is an enemy's ship.
 */
#define FIELD_ENEMY 4

/*
 * Should be called after registering all objects to start a new register-cycle.
 * If a object has moved to another position either the previos positions
 * can be registered with NULL and FIELD_NOTHING or this function can be called
 * and all other objects have to be registered again.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void init_register(void);

/*
 * Should be called in the move-functions to reserve the actual positions of the object.
 * If a position that should be registered is already occupied this function will handle the collision.
 * If both objects are equal shots one collision_shot will be called, if the shots are different both collision_shot will be called.
 * If both objects are ships both collision_ship will be called.
 * If one object is a ship and the other one a shot of the other ship-type collision_shot and collision_ship will be called.
 * The coordinate parameters must be inside the field (between 0 and FIELD_HEIGHT or FIELD_LENGTH).
 * type is one of the FIELD_-constants.
 * <<<INPUT>>>
 * (int)		x:	The x-coordinate, where the object should be registered.
 * (int)		y:	The y-coordinate, where the object should be registered.
 * (void **)	objectadress:		The adress of the object.
 * (int)		type:				The type of the object.
 * <<<OUTPUT>>>
 * (void)
 */
void register_object(int x, int y, void **objectadress, int type);

/*
 * Returns the points of the player.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (int)	The points of the player.
 */
int points_player(void);

/*
 * Increases the points of the player if the value is positive.
 * <<<INPUT>>>
 * (int)	value:	The new points made by the player.
 * <<<OUTPUT>>>
 * (void)
 */
void increase_points_player(int value);

#endif
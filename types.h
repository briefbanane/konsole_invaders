/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		3.09.2011
 * Last change:	22.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

/*
 * Scope:	Declares general constants and the objecttypes shot and ship.
 */

#ifndef __TYPES_H__
#define __TYPES_H__

typedef struct ship_shot *shot;

typedef struct spaceship *ship;

/*
 * The maximum length of the field.
 */
#define MAX_LENGTH 200

/*
 * The maximum height of the field.
 */
#define MAX_HEIGHT 100

/*
 * The number of rows with enemies.
 */
#define ENEMY_ROWS 3
/*
 * The maximum number of columns with enemies.
 */
#define ENEMY_COLUMNS_MAX 25

/*
 * Shows that the game was lost.
 */
#define GAME_LOST 0
/*
 * Shows that all enemies of the actual wave were beaten.
 */
#define GAME_WAVE_BEATEN 1
/*
 * Shows that neither GAME_LOST and GAME_WAVE_BEATEN happened.
 */
#define GAME_NOTHING 2

#endif

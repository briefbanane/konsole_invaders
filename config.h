/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		7.09.2011
 * Last change:	15.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

/*
 * Scope:		Makes the game-relevant constants of the menu-module visible everywhere.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/*
 * The Height of the field.
 */
extern const int field_height;
/*
 * The Length of the field.
 */
extern const int field_length;

/*
 * The number of enemies per line.
 */
extern const int enemy_columns;
/*
 * The number of waves.
 */
extern const int number_of_waves;
/*
 * If it is 1, the player gets a life and a rocket more every second wave.
 */
extern const int life;

#endif
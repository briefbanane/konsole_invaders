/*
 * Copyright:	Fabian Ebner (2011)
 * Author:		Fabian Ebner
 * Licence:		GPL 3.0 or higher
 * Created:		22.09.2011
 * Last change:	21.10.2011
 * This is free software. You can redistribute it under certain conditions (see LICENSE)
 */

/*
 * Scope:	Gives the possibility to configure the game with a menu. Also the configuration can be saved and
 * 			read at the next start of the game with read_configuration.
 */

#ifndef __MENU_H__
#define __MENU_H__

/*
 * Shows a menu in which the user can choose between play, configuration and quit.
 * If the user wants to configure the game another menu will appear where the user can set the game-relevant parameters.
 * This will also be saved in the textfile .konsole_invaders.conf.
 * If the user chooses quit, the program terminates whith exit().
 * The input-thread has to be started before and the function read_configuration should be called before.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void menu(void);

/*
 * Reads the configuration from the file .konsole_invaders.conf.
 * If the file contains illegal values, standard parameters will be used instead.
 * <<<INPUT>>>
 * (void)
 * <<<OUTPUT>>>
 * (void)
 */
void read_configuration(void);

#endif
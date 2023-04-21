/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 04-01-2023
 * Modification : 10-01-2023*/

#include <MLV/MLV_all.h>
#include <stdlib.h>
#include "guardians.h"
#include "player.h"
#include "terrain.h"
#include "score.h"

#define W_WIDTH (C_SIZE * 60) + 59   /* window width */
#define W_HEIGHT (C_SIZE * 45) + 44  /* window height */

#ifndef __GRAPHIC__
#define __GRAPHIC__

/**
 * @brief initializes the window
 * 
 */
void init_window();

/**
 * @brief displays the menu and lets the user interact with it
 * 
 * @param title 
 * @param jouer 
 * @param options 
 * @param nb_gardiens 
 * @param nb_reliques 
 */
void menu(MLV_Image* title, MLV_Image* jouer, MLV_Image* options, int* nb_guardians, int* nb_relics);

/**
 * @brief diplays the game on the open window
 * 
 * @param area 
 * @param player 
 * @param time 
 */
void display_area(Status** area, Player player, clock_t time);

/**
 * @brief displays the player on the field
 * 
 * @param player 
 */
void display_player(Player player);

/**
 * @brief displays all the guardians on the board
 * 
 * @param guardians 
 * @param number 
 */
void display_guardians(Guardian* guardians, int number);

/**
 * @brief displays the scoreboard at the end of the game
 * 
 * @param win 
 */
void scoreboard(int win);

#endif
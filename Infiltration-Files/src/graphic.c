/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 03-01-2023
 * Modification : 10-01-2023*/

#include"../include/graphic.h"

/**
 * @brief initialize the window
 * 
 */
void init_window(){
    /* Opens game window */
    MLV_create_window("Infiltration", "Infiltration", W_WIDTH, W_HEIGHT + 40);
}

/**
 * @brief displays the menu on the opened window
 * 
 * @param title 
 * @param jouer 
 * @param options 
 * @param choice 
 */
static void display_menu(MLV_Image* title, MLV_Image* jouer, MLV_Image* options, int choice) {
    MLV_draw_filled_rectangle(0, 0, W_WIDTH, W_HEIGHT + 100, MLV_COLOR_DARK_GREY);
    if (choice == 1)
        MLV_draw_filled_rectangle(347, 344, 274, 87, MLV_COLOR_BLACK);
    else if (choice == 2)
        MLV_draw_filled_rectangle(347, 544, 274, 87, MLV_COLOR_BLACK);
    MLV_draw_image(title, 200, 100);
    MLV_draw_image(jouer, 353, 350);
    MLV_draw_image(options, 353, 550);
    MLV_actualise_window();
}

/**
 * @brief displays options on the opened window
 * 
 * @param title 
 * @param choice
 * @param nb_gardiens 
 * @param nb_reliques 
 */
static void display_options(MLV_Image* title, int choice, int* nb_guardians, int* nb_relics) {
    MLV_draw_filled_rectangle(0, 0, W_WIDTH, W_HEIGHT, MLV_COLOR_DARK_GREY);
    if (choice == 1)
        MLV_draw_filled_circle(417, 350, 5, MLV_COLOR_BLACK);
    else if (choice == 2)
        MLV_draw_filled_circle(417, 450, 5, MLV_COLOR_BLACK);
    MLV_draw_text(437, 344, "Gardiens < %d >", MLV_COLOR_BLACK, *nb_guardians);
    MLV_draw_text(437, 444, "Reliques < %d >", MLV_COLOR_BLACK, *nb_relics);
    MLV_draw_text(377, 544, "Press R to go to the main menu", MLV_COLOR_BLACK);
    MLV_draw_image(title, 200, 100);
    MLV_actualise_window();
}

/**
 * @brief lets the user change game options
 * 
 * @param title 
 * @param jouer 
 * @param nb_gardiens 
 * @param nb_reliques 
 */
static void options_menu(MLV_Image* title, MLV_Image* jouer, int* nb_guardians, int* nb_relics) {
    MLV_Keyboard_button button;
    MLV_Button_state state;
    int choice;
    choice = 1;
    display_options(title, 1, nb_guardians, nb_relics);
    while(1) {
        MLV_wait_event(&button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &state);
        if (state == MLV_RELEASED) {
            switch(button) {
                case MLV_KEYBOARD_UP :    /*UP*/
                    choice = 1;
                    break;
                case MLV_KEYBOARD_DOWN :  /* DOWN */
                    choice = 2;
                    break;
                case MLV_KEYBOARD_RIGHT : /* RIGHT */
                    if(choice == 1)
                        *nb_guardians += 1;
                    if(choice == 2)
                        *nb_relics += 1;
                    break;
                case MLV_KEYBOARD_LEFT :  /* LEFT */
                    if(choice == 1)
                        *nb_guardians -= 1;
                    if(choice == 2)
                        *nb_relics -= 1;
                    break;
                case MLV_KEYBOARD_r :     /* EXIT */
                    return;
                default : ;
            }
        }
        if(*nb_guardians > 10)
            *nb_guardians = 10;
        if(*nb_guardians < 3)
            *nb_guardians = 3;
        if(*nb_relics > 5)
            *nb_relics = 5;
        if(*nb_relics < 2)
            *nb_relics = 2;
        display_options(title, choice, nb_guardians, nb_relics);
    }
}

/**
 * @brief displays the menu and lets the user interact with it
 * 
 * @param title 
 * @param jouer 
 * @param options 
 * @param nb_gardiens 
 * @param nb_reliques 
 */
void menu(MLV_Image* title, MLV_Image* jouer, MLV_Image* options, int* nb_guardians, int* nb_relics) {
    int choice;
    choice = 1;
    display_menu(title, jouer, options, 1);
    while(1) {
        if (MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_PRESSED) {
            display_menu(title, jouer, options, 1);
            choice = 1;
        }
        if (MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED) {
            display_menu(title, jouer, options, 2);
            choice = 2;
        }
        if (MLV_get_keyboard_state(MLV_KEYBOARD_RETURN) == MLV_PRESSED) {
            if(choice == 1)
                return;
            else if (choice == 2) {
                options_menu(title, jouer, nb_guardians, nb_relics);
                display_menu(title, jouer, options, -1);
                choice = 0;
            }
        }
    }
}

/**
 * @brief diplays the game on the opened window
 * 
 * @param area 
 * @param player 
 * @param time 
 */
void display_area(Status** area, Player player, clock_t time) {
    /* Dessine le plateau de jeu bloc par bloc */
    int x, y;
    int xspacing, yspacing;
    MLV_draw_filled_rectangle(0, 0, W_WIDTH, W_HEIGHT + 40, MLV_COLOR_BLACK);
    for(x = 0; x < 60; x++){
        for(y = 0; y < 45; y++){
            switch(area[x][y]) {
                case mur :
                    MLV_draw_filled_rectangle(x * C_SIZE + x, y * C_SIZE + y, 15, 15, MLV_rgba(10, 10, 10, 255)); break;
                case mana :
                    MLV_draw_filled_rectangle(x * C_SIZE + x, y * C_SIZE + y, 15, 15, MLV_rgba(80, 130, 210, 255)); break;
                case vide :
                    MLV_draw_filled_rectangle(x * C_SIZE + x, y * C_SIZE + y, 15, 15, MLV_COLOR_GREY); break;
                case relique :
                    MLV_draw_filled_rectangle(x * C_SIZE + x, y * C_SIZE + y, 15, 15, MLV_COLOR_RED); break;
                case relique_ramasse :
                    MLV_draw_filled_rectangle(x * C_SIZE + x, y * C_SIZE + y, 15, 15, MLV_rgba(100, 0, 0, 255)); break;
                case relique_vue :
                    MLV_draw_filled_rectangle(x * C_SIZE + x, y * C_SIZE + y, 15, 15, MLV_rgba(100, 0, 0, 255)); break;
                case entree :
                    MLV_draw_filled_rectangle(x * C_SIZE + x, y * C_SIZE + y, 15, 15, MLV_COLOR_SEA_GREEN); break;
            }
            yspacing++;
        }
        xspacing++;
    }
    MLV_draw_filled_rectangle(C_SIZE + 1, W_HEIGHT + 1, W_WIDTH - (2 * C_SIZE) - 1, 39 - C_SIZE, MLV_COLOR_DARK_GREY);
    MLV_draw_filled_rectangle(C_SIZE + 90, W_HEIGHT + 7, player.mana / 2, 12, MLV_COLOR_DARK_BLUE);
    MLV_draw_text(C_SIZE + 7, W_HEIGHT + 6, "Mana : %d", MLV_COLOR_BLACK, player.mana);
    MLV_draw_text(C_SIZE + 795, W_HEIGHT + 6, "Time : %d seconds", MLV_COLOR_BLACK, time);
}

/**
 * @brief displays the player on the board
 * 
 * @param player 
 */
void display_player(Player player) {
    MLV_draw_filled_circle(player.x, player.y, 8, MLV_COLOR_BLACK);
    if(player.visibility == 1)
    MLV_draw_filled_circle(player.x, player.y, 6, MLV_COLOR_RED);
    else if(player.visibility == 0)
    MLV_draw_filled_circle(player.x, player.y, 6, MLV_COLOR_LIGHT_BLUE);
}

/**
 * @brief displays a guardian on the board
 * 
 * @param guardian 
 */
static void display_guardian(Guardian guardian) {
    if (guardian.panic) {
        MLV_draw_filled_circle(guardian.x, guardian.y, 8, MLV_COLOR_BLACK);
        MLV_draw_filled_circle(guardian.x, guardian.y, 6, MLV_COLOR_RED);
        MLV_draw_circle(guardian.x, guardian.y, C_SIZE * guardian.range + 3, MLV_COLOR_RED);
        MLV_draw_circle(guardian.x, guardian.y, C_SIZE * guardian.range + 4, MLV_COLOR_RED);
        return;
    }
    MLV_draw_filled_circle(guardian.x, guardian.y, 8, MLV_COLOR_BLACK);
    MLV_draw_filled_circle(guardian.x, guardian.y, 6, MLV_COLOR_BLUE);
    MLV_draw_circle(guardian.x, guardian.y, C_SIZE * guardian.range + 3, MLV_COLOR_BLUE);
    MLV_draw_circle(guardian.x, guardian.y, C_SIZE * guardian.range + 4, MLV_COLOR_BLUE);
}

/**
 * @brief displays all the guardians on the board
 * 
 * @param guardians 
 * @param number 
 */
void display_guardians(Guardian* guardians, int number) {
    int i;
    for(i = 0; i < number; i++) {
        display_guardian(guardians[i]);
    }
}

/**
 * @brief displays the scoreboard at the end of the game
 * 
 * @param win 
 */
void scoreboard(int win) {
    Score* players;
    int size, i;
    size = 1;
    players = NULL;
    players = score(&size);
    MLV_clear_window(MLV_COLOR_GRAY);
    qsort(players, size, sizeof(Score), time_comparator);
    if (win)
        MLV_draw_text(410, 30, "YOU WIN !!!", MLV_COLOR_BLACK);
    else
        MLV_draw_text(410, 30, "YOU LOSE !!!", MLV_COLOR_BLACK);
    MLV_draw_text(230, 50, "RANKING BY TIME :", MLV_COLOR_BLACK);
    MLV_draw_text(530, 50, "RANKING BY MANA USED :", MLV_COLOR_BLACK);
    if (players[0].time != 0){
        for (i = 0 ; i < 10 && i < size ; i++) {
            MLV_draw_text(230, (i + 1) * 30 + 80, "%d : %s   time spent : %d", MLV_COLOR_BLACK, i + 1, players[i].name, players[i].time);
        }
        qsort(players, size, sizeof(Score), mana_comparator);
        for (i = 0 ; i < 10 && i < size ; i++) {
            MLV_draw_text(530, (i + 1) * 30 + 80, "%d : %s   mana used : %d", MLV_COLOR_BLACK, i + 1, players[i].name, players[i].used_mana);
        }
    }
    MLV_actualise_window();
    MLV_wait_seconds(10);
    free(players);
}
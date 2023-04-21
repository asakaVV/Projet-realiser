/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 03-01-2023
 * Modification : 10-01-2023*/

#ifndef __PLAYER__
#define __PLAYER__

    #include <MLV/MLV_all.h>
    #include "collisions.h"
    #include "movement.h"

    typedef struct {
        float x;              /* position information */
        float y;              /* position information */
        Direction direction;  /* previous key pressed */
        float speed;          /* actual speed */
        int mana;             /* mana information */
        int visibility;       /* visibility state */
    } Player;

    /**
     * @brief initializes the player informations
     * 
     * @return Player 
     */
    Player init_player(void);

    /**
     * @brief lets the player play by moving and/or casting spell
     * 
     * @param player 
     * @param terrain 
     * @param used_mana 
     */
    void move(Player * player, Status** terrain, int* used_mana);

    /**
     * @brief checks if a player wins
     * 
     * @param player 
     * @param terrain 
     * @return int 
     */
    int player_win(Player player, Status** terrain);

#endif
/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 04-01-2023
 * Modification : 10-01-2023*/

#ifndef __COLLISIONS__
#define __COLLISIONS__

    #include "terrain.h"
    #include <math.h>
    
    /**
     * @brief checks if the player is on a relic case, if he does,
     * the relic is taken
     * 
     * @param terrain 
     * @param x 
     * @param y 
     * @return int 
     */
    int take_relic(Status** terrain, float x, float y);

    /**
     * @brief checks if the player is on a mana case, if he does
     * the mana is taken
     * 
     * @param terrain 
     * @param x 
     * @param y 
     * @return int 
     */
    int take_mana(Status** terrain, float x, float y);

    /**
     * @brief calcultates the limit not to exceed if a wall is near
     * the player
     * 
     * @param x 
     * @param y 
     * @param dir 
     * @return float 
     */
    float wall_idrl(float x, float y, int dir);

    /**
     * @brief calcultates the limit not to exceed if a wall on the
     * diagonal left is near the player
     * 
     * @param x 
     * @param y 
     * @param dir 
     * @param wall 
     * @return float 
     */
    float wall_diagonal_left(float x, float y, int dir, int wall);

    /**
     * @brief calcultates the limit not to exceed if a wall on the
     * diagonal right is near the player
     * 
     * @param x 
     * @param y 
     * @param dir 
     * @param wall 
     * @return float 
     */
    float wall_diagonal_right(float x, float y, int dir, int wall);

    /**
     * @brief checks if wall is near the player
     * 
     * @param terrain 
     * @param x 
     * @param y 
     * @param dir 
     * @return int 
     */
    int collision_wall(Status** terrain, float x, float y, int dir);

    /**
     * @brief checks if a player or a missing relic is seen by a
     * guardian
     * 
     * @param terrain 
     * @param px 
     * @param py 
     * @param gx 
     * @param gy 
     * @param range 
     * @return int 
     */
    int guardian_vision(Status** terrain, float px, float py, float gx, float gy, int range);

#endif
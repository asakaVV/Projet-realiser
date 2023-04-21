/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 04-01-2023
 * Modification : 10-01-2023*/

#ifndef __GUARDIAN__
#define __GUARDIAN__

    #include <MLV/MLV_all.h>
    #include "movement.h"
    #include "collisions.h"

    typedef struct {
        float x;              /* position information */
        float y;              /* position information */
        Direction direction;  /* actual direction */
        int panic;            /* panic mode indicator */
        int range;            /* vision range */
        float speed;          /* actual speed */
    } Guardian;

    /**
     * @brief initializes a list of guardians
     * 
     * @param terrain 
     * @return Guardian 
     */
    Guardian* init_guardians(Status** terrain, int number);

    /**
     * @brief moves all the guardians
     * 
     * @param guardians 
     * @param number 
     * @param terrain 
     * @param panictime 
     */
    void guardians_movements(Guardian* guardians, int number, Status** terrain, float* panictime);

    /**
     * @brief checks if a player is in the sight of a guardian
     * 
     * @param terrain 
     * @param x 
     * @param y 
     * @param visibility 
     * @param guardians 
     * @param g_number 
     * @return int 
     */
    int guardians_vision(Status** terrain, float x, float y, int visibility, Guardian* guardians, int g_number);

    /**
     * @brief checks if a guardian sees a missing relic, then turn on panic mode if he does
     * 
     * @param guardians 
     * @param number 
     * @param terrain 
     * @return int 
     */
    int guardians_panic(Guardian* guardians, int number, Status** terrain);

#endif
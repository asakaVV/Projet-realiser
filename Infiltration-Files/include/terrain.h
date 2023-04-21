/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 03-01-2023
 * Modification : 10-01-2023*/

#ifndef __TERRAIN__
#define __TERRAIN__

    #include <stdlib.h>
    #include <time.h>
    #define SPACING 1  /* lines thickness between cases */
    #define C_SIZE 15  /* cases size */
    #define HEIGHT 60  /* number of cases in height */
    #define WIDTH 45   /* number of cases in width */
    #define MINSIDE 9  /* minimum size of a room */

    typedef enum {
        mur,
        mana,
        vide,
        relique,
        relique_ramasse,
        entree,
        relique_vue
    } Status;

    /**
     * @brief initializes the board with mana and walls on the sides
     * 
     * @return Status** 
     */
    Status** init_terrain(void);

    /**
     * @brief random wall generation
     * 
     * @param terrain 
     * @param x1 
     * @param x2 
     * @param y1 
     * @param y2 
     */
    void gen_wall(Status** terrain, int x1, int x2, int y1, int y2);

    /**
     * @brief checks if a wall is behind, on the side or in front of
     * the coodinates
     * 
     * @param terrain 
     * @param x 
     * @param y 
     * @return int 
     */
    int check_near_wall(Status** terrain, int x, int y);

    /**
     * @brief puts given number of relics on the field
     * 
     * @param terrain 
     * @param nb_relic 
     */
    void place_relic(Status** terrain, int nb_relic);

#endif
/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 03-01-2023
 * Modification : 10-01-2023*/

#include "../include/terrain.h"

/**
 * @brief initializes the board with mana and walls on the sides
 * 
 * @return Status** 
 */
Status** init_terrain(void) {
    Status** terrain;
    int i, j;
    /* Initialisation du tableau */
    terrain = (Status**)malloc(sizeof(Status*) * HEIGHT);

    for (i = 0; i < HEIGHT; i++) {
        terrain[i] = (Status*)malloc(sizeof(Status) * WIDTH);
    }
    /* Creation des murs qui entoure le terrain */
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1)
                terrain[i][j] = mur;
            else if (j == 0 || j == WIDTH - 1)
                terrain [i][j] = mur;
            else
                terrain[i][j] = mana;
        }
    }
    terrain[1][1] = entree;
    terrain[1][2] = entree;
    terrain[2][1] = entree;
    terrain[2][2] = entree;
    return terrain;
}

/**
 * @brief puts a wall on the board with an entrance
 * 
 * @param terrain 
 * @param x1 
 * @param x2 
 * @param y1 
 * @param y2 
 * @param wall 
 * @param entree 
 */
static void place_wall(Status** terrain, int x1, int x2, int y1, int y2, int wall, int entree) {
    int inf1 = y1, inf2 = y2, i;
    if (x2 - x1 < y2 - y1){
        inf1 = x1;
        inf2 = x2;
    }
    for (i  = inf1 + 3 * entree; i < inf2 - abs(entree - 1) * 3; i++) {
        if (inf2 - inf1 == y2 - y1)
            terrain[wall][i] = mur;
        else
            terrain[i][wall] = mur;
    }
}

/**
 * @brief random wall generation
 * 
 * @param terrain 
 * @param x1 
 * @param x2 
 * @param y1 
 * @param y2 
 */
void gen_wall(Status** terrain, int x1, int x2, int y1, int y2) {
    int separation, entree;
    int sup = x2 - x1;
    int min = x1, max = x2;
    if (x2 - x1 < y2 - y1) {
        sup = y2 - y1;
        min = y1;
        max = y2;
    }
    if (sup < 2 * MINSIDE + 1)
        return;
    if (sup < 4 * MINSIDE) {
        return;
    }
    do {
        separation = rand() % max - MINSIDE;
    } while(separation < min + MINSIDE);
    
    entree = rand() % 2;
    place_wall(terrain, x1, x2, y1, y2, separation, entree);
    if (sup == x2 - x1) {
        gen_wall(terrain, separation + 1, x2, y1, y2);
        gen_wall(terrain, x1, separation, y1, y2);
    }
    else {
        gen_wall(terrain, x1, x2, separation + 1, y2);
        gen_wall(terrain, x1, x2, y1, separation);
    }
}

/**
 * @brief checks if a wall is behind, on the side or in front of
 * the coordinates
 * 
 * @param terrain 
 * @param x 
 * @param y 
 * @return int
 */
int check_near_wall(Status** terrain, int x, int y) { 
    if (terrain[x + 1][y] == mur)
        return 1;
    if (terrain[x - 1][y] == mur)
        return 1;
    if (terrain[x][y + 1] == mur)
        return 1;
    if (terrain[x][y - 1] == mur)
        return 1;
    return 0;
}

/**
 * @brief puts given number of relics on the field
 * 
 * @param terrain 
 * @param nb_relic 
 */
void place_relic(Status** terrain, int nb_relic) {
    int x, y, i;
    for (i = 0; i < nb_relic; i++){
        do {
            x = (rand() % (HEIGHT - 2)) + 1;
            y = (rand() % (WIDTH - 2)) + 1;
        } while (terrain[x][y] == relique || terrain[x][y] == mur || check_near_wall(terrain, x, y));
        terrain[x][y] = relique;
    }
}
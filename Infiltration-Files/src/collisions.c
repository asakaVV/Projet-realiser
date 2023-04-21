/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 04-01-2023
 * Modification : 10-01-2023*/

#include "../include/collisions.h"
#include <MLV/MLV_all.h>
#include <stdio.h>

/**
 * @brief checks if the player is on a relic case, if he does
 * the relic is taken
 * 
 * @param terrain 
 * @param x 
 * @param y 
 * @return int 
 */
int take_relic(Status** terrain, float x, float y) {
    int x_tab = x / (C_SIZE + 1), y_tab = y / (C_SIZE + 1);
    if (terrain[x_tab][y_tab] == relique) {
        terrain[x_tab][y_tab] = relique_ramasse;
        return 1;
    }
    return 0;
}

/**
 * @brief check if the player is on a mana case, if he does
 * the mana is taken
 * 
 * @param terrain 
 * @param x 
 * @param y 
 * @return int 
 */
int take_mana(Status** terrain, float x, float y) {
    int x_tab = x / (C_SIZE + 1), y_tab = y / (C_SIZE + 1);
    if (terrain[x_tab][y_tab] == mana) {
        terrain[x_tab][y_tab] = vide;
        return 1;
    }
    return 0;
}

/**
 * @brief calcultates the limit not to exceed if a wall is near
 * the player
 * 
 * @param x 
 * @param y 
 * @param dir 
 * @return float 
 */
float wall_idrl(float x, float y, int dir) {
    if (dir == 1 || dir == 2) 
        return ((int)(y / (C_SIZE + SPACING)) * (C_SIZE + SPACING)) + ((C_SIZE + SPACING) / 2);
    if (dir == 3 || dir == 4)
        return ((int)(x / (C_SIZE + SPACING)) * (C_SIZE + SPACING)) + ((C_SIZE + SPACING) / 2);
    return 0;
}

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
float wall_diagonal_left(float x, float y, int dir, int wall) {
    int size;
    size = C_SIZE + SPACING;
    if (dir == 1) {
        if (0.25 - ((x / size) - (int)(x / size)) * ((x / size) - (int)(x / size)) > 0)
            return ((int)(y / size) * size) + (sqrt(0.25 - ((x / size) - (int)(x / size)) * ((x / size) - (int)(x / size))) * size);
    }
    if (dir == 2) {
        if (0.25 - ((x / size) - (int)(x / size)) * ((x / size) - (int)(x / size)) > 0)
            return ((int)(y / size) * size) + size - (sqrt(0.25 - ((x / size) - (int)(x / size)) * ((x / size) - (int)(x / size))) * size);
    }
    if (dir == 4) {
        float dist;
        if (wall) {
            dist = (((y / size) - ((int)((y / size)))) * ((y / size) - ((int)((y / size)))));
        }
        else {
            dist = ((((int)((y / size))) + 1 - (y / size)) * (((int)((y / size))) + 1 - (y / size)));
        }
        if (0.25 - dist > 0)
            return ((int)(x / size) * size) + (sqrt(0.25 - dist) * size);
    }
    return 0;
}

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
float wall_diagonal_right(float x, float y, int dir, int wall) {
    int size;
    size = C_SIZE + SPACING;
    if (dir == 1) {
        if (0.25 - ((int)(x / size) + 1 - (x / size)) * ((int)(x / size) + 1 - (x / size)) > 0)
            return ((int)(y / size) * size) + (sqrt(0.25 - ((int)(x / size) + 1 - (x / size)) * ((int)(x / size) + 1 - (x / size))) * size);
    }
    if (dir == 2) {
        if (0.25 - ((int)(x / size) + 1 - (x / size)) * ((int)(x / size) + 1 - (x / size)) > 0)
            return ((int)(y / size) * size) + size - (sqrt(0.25 - ((int)(x / size) + 1 - (x / size)) * ((int)(x / size) + 1 - (x / size))) * size);
    }
    if (dir == 3) {
        float dist;
        if (wall) {
            dist = (((y / size) - ((int)((y / size)))) * ((y / size) - ((int)((y / size)))));
        }
        else {
            dist = ((((int)((y / size))) + 1 - (y / size)) * (((int)((y / size))) + 1 - (y / size)));
        }
        if (0.25 - dist > 0)
            return ((int)(x / size) * size) + size - (sqrt(0.25 - dist) * size);
    }
    return 0;
}

/**
 * @brief checks if a wall is near the player
 * 
 * @param terrain 
 * @param x 
 * @param y 
 * @param dir 
 * @return int 
 */
int collision_wall(Status** terrain, float x, float y, int dir) {
    int x_tab = x / (C_SIZE + 1), y_tab = y / (C_SIZE + 1);
    if (terrain[x_tab][y_tab - 1] == mur && dir == 1) 
        return 1;
    if (terrain[x_tab][y_tab + 1] == mur && dir == 2) 
        return 2;
    if (terrain[x_tab + 1][y_tab] == mur && dir == 3) 
        return 3;
    if (terrain[x_tab - 1][y_tab] == mur && dir == 4) 
        return 4;
    if (terrain[x_tab - 1][y_tab + 1] == mur)
        return 5;
    if (terrain[x_tab - 1][y_tab - 1] == mur)
        return 6;
    if (terrain[x_tab + 1][y_tab + 1] == mur)
        return 7;
    if (terrain[x_tab + 1][y_tab - 1] == mur)
        return 8;
    return 0;
}

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
int guardian_vision(Status** terrain, float px, float py, float gx, float gy, int range) {
    int c_dist_x, c_dist_y, x, y, ya, yb, a, b, size, x_sign, y_sign;
    float dist;
    x_sign = 1;
    y_sign = 1;
    x = 0;
    y = 0;
    size = (C_SIZE + 1);
    c_dist_x = abs((int)px/size - (int)gx/size);
    c_dist_y = abs((int)py/size - (int)gy/size);
    if(gx > px)
        x_sign = -1;
    else {
        x++;
        c_dist_x ++;
    }
    if(gy > py)
        y_sign = -1;
    else {
        y++;
        c_dist_y ++;
    }
    dist = sqrt(pow(px/size - gx/size, 2) + pow(py/size - gy/size, 2));

    for(; x < c_dist_x; x++) {
        a = gx/size + x * x_sign;
        ya = gy/size + (py/size - gy/size) / (px/size - gx/size) * (a - gx/size);
        if(dist < range) {
            if(terrain[a][ya] == mur)
                return 0;
        }
    }
    for(; y < c_dist_y; y++) {
        b = gy/size + y * y_sign;
        yb = gx/size + (px/size - gx/size)/ (py/size - gy/size) * (b - gy/size);
        if(dist < range) {
            if(terrain[yb][b] == mur)
                return 0;
        }
    }
    if(dist < range)
        return 1;
    return 0;
}

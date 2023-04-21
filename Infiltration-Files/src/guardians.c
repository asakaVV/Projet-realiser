/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 04-01-2023
 * Modification : 10-01-2023*/

#include "../include/guardians.h"

/**
 * @brief initializes a guardian with position, range, speed and direction
 * 
 * @param terrain
 * @return Guardian
 */
static Guardian init_guardian(Status** terrain) {
    Guardian guardian;
    int x, y;
    do {
        x = (rand() % HEIGHT);
        y = (rand() % WIDTH);
    } while ((x <= 10 && y <= 10)  || terrain[x][y] == relique || terrain[x][y] == mur || check_near_wall(terrain, x, y));
    guardian.x = x * C_SIZE;
    guardian.y = y * C_SIZE;
    guardian.panic = 0;
    guardian.range = 4;
    guardian.speed = ((rand() % 51) + 30);
    guardian.speed = (guardian.speed / 100) * SPEED;
    guardian.direction = (rand() % 4) + 1;
    return guardian;
}

/**
 * @brief initializes a list of guardians
 * 
 * @param terrain 
 * @return Guardian 
 */
Guardian* init_guardians(Status** terrain, int number) {
    Guardian* guardians;
    int i;
    guardians = (Guardian*)malloc(sizeof(Guardian) * number);
    for(i = 0; i < number; i++) {
        guardians[i] = init_guardian(terrain);
    }
    return guardians;
}

/**
 * @brief changes the direction of a guardian
 * 
 * @param guardian 
 */
static void change_direction(Guardian* guardian) {
    int direction;
    direction = rand() % 3;
    switch (guardian->direction) {
    case UP :
        if(direction == 0) guardian->direction = DOWN;
        if(direction == 1) guardian->direction = RIGHT;
        if(direction == 2) guardian->direction = LEFT;
        break;
    case DOWN :
        if(direction == 0) guardian->direction = UP;
        if(direction == 1) guardian->direction = RIGHT;
        if(direction == 2) guardian->direction = LEFT;
        break;
    case RIGHT :
        if(direction == 0) guardian->direction = DOWN;
        if(direction == 1) guardian->direction = UP;
        if(direction == 2) guardian->direction = LEFT;
        break;
    case LEFT :
        if(direction == 0) guardian->direction = DOWN;
        if(direction == 1) guardian->direction = UP;
        if(direction == 2) guardian->direction = RIGHT;
        break;
    default:
        break;
    }
}

/**
 * @brief changes the speed of a guardian
 * 
 * @param guardian 
 */
static void change_speed(Guardian* guardian) {
    guardian->speed = ((rand() % 51) + 30);
    guardian->speed = (guardian->speed / 100) * SPEED;
}

/**
 * @brief guardian movement up
 * 
 * @param guardian 
 * @param terrain 
 */
static void guardian_up(Guardian* guardian, Status** terrain) {
    int collision;
    collision = collision_wall(terrain, guardian->x, guardian->y, UP);
    if (collision == 1) {
        float pos_max = wall_idrl(guardian->x, guardian->y, UP);
        if (guardian->y - guardian->speed <= pos_max + (guardian->panic * (C_SIZE + 1))) {
            change_direction(guardian);
            change_speed(guardian);
            return;
        }
    }
    if (collision == 6) {
        float pos_max = wall_diagonal_left(guardian->x, guardian->y, UP, 0);
        if (guardian->y - guardian->speed <= pos_max + (guardian->panic * (C_SIZE + 1)) && pos_max != 0) {
            change_direction(guardian);
            change_speed(guardian);
            return;
        }
    }
    if (collision == 8) {
        float pos_max = wall_diagonal_right(guardian->x, guardian->y, UP, 0);
        if (guardian->y - guardian->speed <= pos_max + (guardian->panic * (C_SIZE + 1)) && pos_max != 0) {
            change_direction(guardian);
            change_speed(guardian);
            return;
        }
    }
    guardian->y -= guardian->speed;
}

/**
 * @brief guardian movement down
 * 
 * @param guardian 
 * @param terrain 
 */
static void guardian_down(Guardian* guardian, Status** terrain) {
    int collision;
    collision = collision_wall(terrain, guardian->x, guardian->y, DOWN);
    if (collision == 2) {
        float pos_max = wall_idrl(guardian->x, guardian->y, DOWN);
        if (guardian->y + guardian->speed >= pos_max - (guardian->panic * (C_SIZE + 1))) {
            change_direction(guardian);
            change_speed(guardian);
            return;
        }
    }
    if (collision == 5) {
        float pos_max = wall_diagonal_left(guardian->x, guardian->y, DOWN, 0);
        if (guardian->y + guardian->speed >= pos_max - (guardian->panic * (C_SIZE + 1)) && pos_max != 0) {
            change_direction(guardian);
            change_speed(guardian);
            return;
        }
    }
    if (collision == 7) {
        float pos_max = wall_diagonal_right(guardian->x, guardian->y, DOWN, 0);
        if (guardian->y + guardian->speed >= pos_max - (guardian->panic * (C_SIZE + 1)) && pos_max != 0) {
            change_direction(guardian);
            change_speed(guardian);
            return;
        }
    }
    guardian->y += guardian->speed;
}

/**
 * @brief guardian movement right
 * 
 * @param guardian 
 * @param terrain 
 */
static void guardian_right(Guardian* guardian, Status** terrain) {
    int collision;
    collision = collision_wall(terrain, guardian->x, guardian->y, RIGHT);
    if (collision == 3) {
        float pos_max = wall_idrl(guardian->x, guardian->y, RIGHT);
        if (guardian->x + guardian->speed >= pos_max - (guardian->panic * (C_SIZE + 1))) {
            change_direction(guardian);
            change_speed(guardian);
            return;
        }
    }
    if (collision == 7 || collision == 8) {
        float pos_max = wall_diagonal_right(guardian->x, guardian->y, RIGHT, collision - 7);
        if (guardian->x + guardian->speed >= pos_max - (guardian->panic * (C_SIZE + 1)) && pos_max) {
            change_direction(guardian);
            change_speed(guardian);
            return;
        }
    }
    guardian->x += guardian->speed;
}

/**
 * @brief guardian movement left
 * 
 * @param guardian 
 * @param terrain 
 */
static void guardian_left(Guardian* guardian, Status** terrain) {
    int collision;
    collision = collision_wall(terrain, guardian->x, guardian->y, LEFT);
    if (collision == 4) {
        float pos_max = wall_idrl(guardian->x, guardian->y, LEFT);
        if (guardian->x - guardian->speed <= pos_max + (guardian->panic * (C_SIZE + 1))) {
            change_direction(guardian);
            change_speed(guardian);
            return;
        }
    }
    if (collision == 5 || collision == 6) {
        float pos_max = wall_diagonal_left(guardian->x, guardian->y, LEFT, collision - 5);
        if (guardian->x - guardian->speed <= pos_max + (guardian->panic * (C_SIZE + 1)) && pos_max) {
            change_direction(guardian);
            change_speed(guardian);
            return;
        }
    }
    guardian->x -= guardian->speed;
}

/**
 * @brief makes the guardian move on the right direction
 * 
 * @param guardian 
 * @param terrain 
 */
static void guardian_move(Guardian* guardian, Status** terrain) {
    if (guardian->direction == UP)
        guardian_up(guardian, terrain);
    else if (guardian->direction == DOWN)
        guardian_down(guardian, terrain);
    else if (guardian->direction == RIGHT)
        guardian_right(guardian, terrain);
    else if (guardian->direction == LEFT)
        guardian_left(guardian, terrain);
}

/**
 * @brief randomly changes the speed and the direction of a guardian and moves him
 * 
 * @param guardian 
 * @param terrain 
 * @param panictime 
 */
static void guardian_movement(Guardian* guardian, Status** terrain, float* panictime) {
    int direction;
    int speed;
    direction = rand() % 50;
    speed = rand() % 50;
    if (*panictime > -1 && *panictime < 30) {
        guardian->speed = SPEED;
    }
    else {
        *panictime = -1;
        guardian->panic = 0;
        guardian->range = 4;
    }
    if(direction == 0)
        change_direction(guardian);
    if(speed == 0 && !guardian->panic)
        change_speed(guardian);
    guardian_move(guardian, terrain);
}

/**
 * @brief moves all the guardians
 * 
 * @param guardians 
 * @param number 
 * @param terrain 
 * @param panictime 
 */
void guardians_movements(Guardian* guardians, int number, Status** terrain, float* panictime) {
    int i;
    for(i = 0; i < number; i++) {
        guardian_movement(&(guardians[i]), terrain, panictime);
    }
}

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
int guardians_vision(Status** terrain, float x, float y, int visibility, Guardian* guardians, int g_number) {
    int i;
    for (i = 0 ; i < g_number ; i ++) {
        if (guardian_vision(terrain, x, y, guardians[i].x, guardians[i].y, guardians[i].range) && visibility)
            return 1;
    }
    return 0;
}

/**
 * @brief changes the panic information and range of all guardians
 * 
 * @param guardians 
 * @param number 
 */
static void panic_mode(Guardian* guardians, int number) {
    int i;
    for (i = 0 ; i < number ; i++) {
        guardians[i].panic = 1;
        guardians[i].range = 6;
    }
}

/**
 * @brief checks if a guardian sees a missing relic, then turn on panic mode if he does
 * 
 * @param guardians 
 * @param number 
 * @param terrain 
 * @return int 
 */
int guardians_panic(Guardian* guardians, int number, Status** terrain) {
    int i, j, k;
    for (i = 0 ; i < HEIGHT ; i++) {
        for (j = 0 ; j < WIDTH ; j++) {
            if (terrain[i][j] == relique_ramasse) {
                for (k = 0 ; k < number ; k++) {
                    if (guardian_vision(terrain, i * (C_SIZE + 1), j * (C_SIZE + 1), guardians[k].x, guardians[k].y, guardians[k].range)) {
                        panic_mode(guardians, number);
                        terrain[i][j] = relique_vue;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
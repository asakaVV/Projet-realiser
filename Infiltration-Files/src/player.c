/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 03-01-2023
 * Modification : 10-01-2023*/

#include "../include/player.h"

/**
 * @brief initializes the player informations
 * 
 * @return Player 
 */
Player init_player(void) {
    Player player;
    player.x = 15 * 2 + 1;
    player.y = 15 * 2 + 1;
    player.speed = 0.1 * SPEED;
    player.mana = 0;
    player.direction = STOP;
    player.visibility = 1;
    return player;
}

/**
 * @brief changes the speed of the player
 * 
 * @param player 
 * @param max_speed 
 */
static void new_speed(Player* player, float max_speed) {
    if (player->speed < max_speed * SPEED) {
        if (player->speed + 0.03 * SPEED > max_speed * SPEED)
            player->speed = max_speed * SPEED;
        else
            player->speed += 0.03 * SPEED;
    }
    else 
        player->speed = max_speed * SPEED;
}

/**
 * @brief moves the player up
 * 
 * @param player 
 * @param max_speed 
 * @param terrain 
 */
static void move_up(Player* player, float max_speed, Status** terrain) {
    int collision;
    collision = collision_wall(terrain, player->x, player->y, UP);
    if (collision == 1) {
        float pos_max = wall_idrl(player->x, player->y, UP);
        if (player->y - player->speed <= pos_max) {
            player->y = pos_max;
            return;
        }
    }
    if (collision == 6) {
        float pos_max = wall_diagonal_left(player->x, player->y, UP, 0);
        if (player->y - player->speed <= pos_max && pos_max != 0) {
            player->y = pos_max;
            return;
        }
    }
    if (collision == 8) {
        float pos_max = wall_diagonal_right(player->x, player->y, UP, 0);
        if (player->y - player->speed <= pos_max && pos_max != 0) {
            player->y = pos_max;
            return;
        }
    }
    player->y -= player->speed;
    if (player->direction != UP) {
        player->direction = UP;
        player->speed = 0.1 * SPEED;
    }
    else
        new_speed(player, max_speed);
}

/**
 * @brief moves the player down
 * 
 * @param player 
 * @param max_speed 
 * @param terrain 
 */
static void move_down(Player* player, float max_speed, Status** terrain) {
    int collision;
    collision = collision_wall(terrain, player->x, player->y, DOWN);
    if (collision == 2) {
        float pos_max = wall_idrl(player->x, player->y, DOWN);
        if (player->y + player->speed >= pos_max) {
            player->y = pos_max;
            return;
        }
    }
    if (collision == 5) {
        float pos_max = wall_diagonal_left(player->x, player->y, DOWN, 0);
        if (player->y + player->speed >= pos_max && pos_max != 0) {
            player->y = pos_max;
            return;
        }
    }
    if (collision == 7) {
        float pos_max = wall_diagonal_right(player->x, player->y, DOWN, 0);
        if (player->y + player->speed >= pos_max && pos_max != 0) {
            player->y = pos_max;
            return;
        }
    }
    player->y += player->speed;
    if (player->direction != DOWN) {
        player->direction = DOWN;
        player->speed = 0.1 * SPEED;
    }
    else
        new_speed(player, max_speed);
}

/**
 * @brief moves the player right
 * 
 * @param player 
 * @param max_speed 
 * @param terrain 
 */
static void move_right(Player* player, float max_speed, Status** terrain) {
    int collision;
    collision = collision_wall(terrain, player->x, player->y, RIGHT);
    if (collision == 3) {
        float pos_max = wall_idrl(player->x, player->y, RIGHT);
        if (player->x + player->speed >= pos_max) {
            player->x = pos_max;
            return;
        }
    }
    if (collision == 7 || collision == 8) {
        float pos_max = wall_diagonal_right(player->x, player->y, RIGHT, collision - 7);
        if (player->x + player->speed >= pos_max && pos_max) {
            player->x = pos_max;
            return;
        }
    }
    player->x += player->speed;
    if (player->direction != RIGHT) {
        player->direction = RIGHT;
        player->speed = 0.1 * SPEED;
    }
    else
        new_speed(player, max_speed);
}

/**
 * @brief moves the player left
 * 
 * @param player 
 * @param max_speed 
 * @param terrain 
 */
static void move_left(Player* player, float max_speed, Status** terrain) {
    int collision;
    collision = collision_wall(terrain, player->x, player->y, LEFT);
    if (collision == 4) {
        float pos_max = wall_idrl(player->x, player->y, LEFT);
        if (player->x - player->speed <= pos_max) {
            player->x = pos_max;
            return;
        }
    }
    if (collision == 5 || collision == 6) {
        float pos_max = wall_diagonal_left(player->x, player->y, LEFT, collision - 5);
        if (player->x - player->speed <= pos_max && pos_max) {
            player->x = pos_max;
            return;
        }
    }
    player->x -= player->speed;
    if (player->direction != LEFT) {
        player->direction = LEFT;
        player->speed = 0.1 * SPEED;
    }
    else
        new_speed(player, max_speed);
}

/**
 * @brief checks the key pressed for the player movement
 * 
 * @return int 
 */
static int direction_key_information() {
    if (MLV_get_keyboard_state(MLV_KEYBOARD_UP) == MLV_PRESSED ||
        MLV_get_keyboard_state(MLV_KEYBOARD_z) == MLV_PRESSED)
            return MLV_KEYBOARD_UP;
    if (MLV_get_keyboard_state(MLV_KEYBOARD_LEFT) == MLV_PRESSED ||
        MLV_get_keyboard_state(MLV_KEYBOARD_q) == MLV_PRESSED)
            return MLV_KEYBOARD_LEFT;
    if (MLV_get_keyboard_state(MLV_KEYBOARD_DOWN) == MLV_PRESSED ||
        MLV_get_keyboard_state(MLV_KEYBOARD_s) == MLV_PRESSED)
            return MLV_KEYBOARD_DOWN;
    if (MLV_get_keyboard_state(MLV_KEYBOARD_RIGHT) == MLV_PRESSED ||
        MLV_get_keyboard_state(MLV_KEYBOARD_d) == MLV_PRESSED)
            return MLV_KEYBOARD_RIGHT;
    return 0;
}

/**
 * @brief checks the key pressed for the player spell
 * 
 * @param player 
 * @return int 
 */
static int spell_key_information(Player* player) {
    if(player->mana > 0) {
        if (MLV_get_keyboard_state(MLV_KEYBOARD_SPACE) == MLV_PRESSED)
                return MLV_KEYBOARD_SPACE;
    }
    if(player->mana > 1) {
        if (MLV_get_keyboard_state(MLV_KEYBOARD_LSHIFT) == MLV_PRESSED ||
            MLV_get_keyboard_state(MLV_KEYBOARD_RSHIFT) == MLV_PRESSED)
                return MLV_KEYBOARD_LSHIFT;
    }
    return 0;
}

/**
 * @brief replaces the mana on the field when the player casts a spell
 * 
 * @param terrain 
 * @param player
 * @param number 
 */
static void return_mana(Status** terrain, Player player, int number) {
    int n, i, j, rand_num, case_num;
    for(n = 0; n <= number; n++) {
        case_num = -1;
        rand_num = rand() % (player.mana + 1);
        for (i = 0 ; i < HEIGHT ; i++) {
            for (j = 0 ; j < WIDTH ; j++) {
                if(terrain[i][j] == vide && (i != (int)player.x/(C_SIZE + 1) || j != (int)player.y/(C_SIZE + 1))) {
                    case_num++;
                    if(case_num == rand_num) {
                        terrain[i][j] = mana;
                    }
                }
            }
        }
    }
}

/**
 * @brief detects which spell is cast
 * 
 * @param terrain 
 * @param player 
 * @param max_speed 
 * @param used_mana 
 */
static void spell_detection(Status** terrain, Player* player, float* max_speed, int* used_mana) {
    int spell_key;
    spell_key = spell_key_information(player);
    switch (spell_key) {
        case MLV_KEYBOARD_SPACE:   /* Invisibility */
            player->visibility = 0;
            *used_mana += 1;
            player->mana--;
            return_mana(terrain, *player, 1);
            break;
        case MLV_KEYBOARD_LSHIFT:  /* Speed Up */
            *max_speed = 1.2;
            *used_mana += 2;
            player->mana -= 2;
            if (player->mana < 0)
                player->mana = 0;

            return_mana(terrain, *player, 2);
            break;
        default: 
            player->visibility = 1;
            *max_speed = 0.9;
    }
}

/**
 * @brief detects which direction is chosen
 * 
 * @param player 
 * @param max_speed 
 * @param terrain 
 */
static void direction_detection(Player* player, float max_speed, Status** terrain) {
    int key;
    key = direction_key_information();
    switch (key) {
        case MLV_KEYBOARD_UP:     /* UP */
            move_up(player, max_speed, terrain);
            break;
        case MLV_KEYBOARD_DOWN:   /* DOWN */
            move_down(player, max_speed, terrain);
            break;
        case MLV_KEYBOARD_RIGHT:  /* RIGHT */
            move_right(player, max_speed, terrain);
            break;   
        case MLV_KEYBOARD_LEFT:   /* LEFT */
            move_left(player, max_speed, terrain);
            break;    
        default:
            player->direction = -1;
            player->speed = 0.1 * SPEED;
            break;
    }
}

/**
 * @brief lets the player play by moving and/or casting spell
 * 
 * @param player 
 * @param terrain 
 * @param used_mana 
 */
void move(Player* player, Status** terrain, int* used_mana) {
    float max_speed;
    max_speed = 0.9;
    /* Spell Detection */
    spell_detection(terrain, player, &max_speed, used_mana);
    /* Direction Detection */
    direction_detection(player, max_speed, terrain);
}

/**
 * @brief checks if a player win
 * 
 * @param player 
 * @param terrain 
 * @return int 
 */
int player_win(Player player, Status** terrain) {
    if (player.x > 1 * (C_SIZE + 1) && player.x < 3 * (C_SIZE + 1) &&
        player.y > 1 * (C_SIZE + 1) && player.y < 3 * (C_SIZE + 1)){
            int i, j;
            for (i = 0 ; i < HEIGHT ; i++) {
                for (j = 0 ; j < WIDTH ; j++) {
                    if (terrain[i][j] == relique) 
                        return 0;
                }
            }
            return 1;
        }
    return 0;
}
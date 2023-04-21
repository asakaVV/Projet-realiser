/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 03-01-2023
 * Modification : 10-01-2023*/

#define _POSIX_C_SOURCE 199309L

#include "../include/graphic.h"
#include "../include/collisions.h"
#include "../include/player.h"
#include "../include/guardians.h"
#include "../include/score.h"
#include <time.h>

int main () {
    Status** terrain;
    Player player;
    Guardian* guardians;
    MLV_Image* title;
    MLV_Image* jouer;
    MLV_Image* options;
    struct timespec end_time;
    struct timespec new_time;
    struct timespec start;
    struct timespec actual;
    struct timespec panicstart;
    struct timespec panicend;
    float frametime;
    float extratime;
    float actualtime;
    float panictime;
    int win, lose;
    int used_mana;
    int nb_guardians, nb_relics;

    srand(time(NULL));

    init_window();

    /* Terrain initialisation */
    terrain = init_terrain();
    gen_wall(terrain, 1, HEIGHT - 1, 1, WIDTH - 1);

    nb_guardians = 5;
    nb_relics = 3;
    

    /* Images load */
    title = MLV_load_image("./images/title.png");
    jouer = MLV_load_image("./images/jouer.png");
    options = MLV_load_image("./images/options.png");

    /* Menu */
    menu(title, jouer, options, &nb_guardians, &nb_relics);

    MLV_free_image(title);
    MLV_free_image(jouer);
    MLV_free_image(options);

    /* Game */
    place_relic(terrain, nb_relics);
    player = init_player();
    guardians = init_guardians(terrain, nb_guardians);
    win = 0;
    lose = 0;
    clock_gettime(CLOCK_REALTIME, &start);
    panictime = -1;
    used_mana = 0;

    /* Main loop */
    while(!(win + lose)) {

        /* Get the time in nanoscond at the start of the frame */
        clock_gettime(CLOCK_REALTIME, &end_time);
        clock_gettime(CLOCK_REALTIME, &actual);
        actualtime = actual.tv_sec - start.tv_sec;

        /* Display of the current frame*/
        display_area(terrain, player, actualtime);
        display_player(player);
        display_guardians(guardians, nb_guardians);

        /*Dealing with the events*/

        /*Move the entities on the grid*/
        move(&player, terrain, &used_mana);
        guardians_movements(guardians, nb_guardians, terrain, &panictime);

        /*Collision detection and other game mechanisms*/
        take_relic(terrain, player.x, player. y);
        player.mana += take_mana(terrain, player.x, player. y);
        if (guardians_panic(guardians, nb_guardians, terrain)) {
            clock_gettime(CLOCK_REALTIME, &panicstart);
        }
        if (guardians[0].panic) {
            clock_gettime(CLOCK_REALTIME, &panicend);
            panictime = panicend.tv_sec - panicstart.tv_sec;
        }

        win += player_win(player, terrain);
        lose += guardians_vision(terrain, player.x, player.y, player.visibility, guardians, nb_guardians);
        MLV_actualise_window();


        /*Get the time in nanosecond at the end of the frame*/
        clock_gettime(CLOCK_REALTIME, &new_time);

        /*Compute  the  time  spent  for  the  current  frame*/
        frametime = new_time.tv_sec - end_time.tv_sec;
        frametime += (new_time.tv_sec - end_time.tv_sec) / 1.0E9;

        /*Force the program to spend at least 1/60 second in total*/
        extratime = 1.0 / 60 - frametime;
        if(extratime > 0) {
            MLV_wait_milliseconds((int)(extratime * 1000));
        }
    }
    if (win)
        save_score((int)actualtime, used_mana);
    scoreboard(win);
    free(terrain);
    free(guardians);
    MLV_free_window();

    return 0;
}

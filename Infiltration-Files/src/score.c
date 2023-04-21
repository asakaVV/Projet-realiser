/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 07-01-2023
 * Modification : 10-01-2023*/

#include "../include/score.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief comparator of time
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int time_comparator(const void* a, const void* b) {
    Score player1 = * (const Score*)a;
    Score player2 = * (const Score*)b;
    return player1.time - player2.time;
}

/**
 * @brief comparator of mana
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int mana_comparator(const void* a, const void* b) {
    Score player1 = * (const Score*)a;
    Score player2 = * (const Score*)b;
    return player1.used_mana - player2.used_mana;
}

/**
 * @brief saves the score in a binary file
 * 
 * @param time 
 * @param used_mana 
 */
void save_score(int time, int used_mana) {
    Score player;
    FILE* file;
    file = fopen("./scores", "ab");
    printf("Enter your name (3 characters) : ");

    /* Results saving */
    scanf("%s", player.name);
    player.name[3] = '\0';  /* forces length of 3 */
    player.time = time;
    player.used_mana = used_mana;
    
    fwrite(&player, sizeof(Score), 1, file);
    
    fclose(file);
}

/**
 * @brief recovers the score for a binary file
 * 
 * @param size 
 * @return Score* 
 */
Score* score(int* size) {
    Score player;
    Score* all_player;
    FILE* file;
    int i;
    all_player = (Score*)malloc(sizeof(Score) * (*size));
    i = 0;
    file = fopen("./scores", "rb");
    while (fread(&player, sizeof(Score), 1, file)) {
        if (i == *size) {
            *size *= 2;
            all_player = (Score*)realloc(all_player, sizeof(Score) * (*size));
        }
        all_player[i] = player;
        i++; 
        }
    fclose(file);
    return all_player;
}

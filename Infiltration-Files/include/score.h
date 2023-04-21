/* Auteur : FORBIN Romain, MENLET Enzo
 * Creation : 07-01-2023
 * Modification : 10-01-2023*/

#ifndef __SCORE__
#define __SCORE__

    typedef struct {
        char name[4];
        int used_mana;
        int time;
    } Score;

    /**
     * @brief comparator of time
     * 
     * @param a 
     * @param b 
     * @return int 
     */
    int time_comparator(const void* a, const void* b);

    /**
     * @brief comparator of mana
     * 
     * @param a 
     * @param b 
     * @return int 
     */
    int mana_comparator(const void* a, const void* b);

    /**
     * @brief saves the score in a binary file
     * 
     * @param time 
     * @param used_mana 
     */
    void save_score(int time, int used_mana);

    /**
     * @brief recovers the score from a binary file
     * 
     * @param size 
     * @return Score* 
     */
    Score* score(int* size);

#endif
/*********************************************
*            Projet Tower Defense            *
*                Forbin Romain               *
*                Fraize Victor               *
*********************************************/          
#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#define TAILLE_FENETRE_X 1780
#define TAILLE_FENETRE_Y 1000
#define TAILLE_MARGE_X 260
#define TAILLE_MARGE_Y 200
#define TAILLE_CASE 60
#define ESPACE_CASE 50

/* Définition des structures */
//Struct Defenseur
typedef struct defenseur{
    int type;
    int life;
    int max_life;
    int line;
    int position;
    int price;
    int damage;
    struct defenseur* next;
}Defenseur;

//Struct Ennemis
typedef struct ennemis{
    char type;
    int life;
    int max_life;
    int line;
    int position;
    int speed;
    int turn;
    struct ennemis* next;
    struct ennemis* next_line;
    struct ennemis* prev_line;
}Ennemis;

//Struct Game
typedef struct{
    Ennemis* ennemis;
    Defenseur* defenseur;
    int turn;
    int money;
}Game;

//Struct Plateau
typedef struct{
    Ennemis* ennemis[7][26];
    Defenseur* tourelles[7][26];
}Plateau;


/* INITIALISATION DES STRUCTURES */
/* 1) Struct ennemis */

/* Stats des différents ennemis */
void initChevalier(Ennemis *chevalier){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - chevalier : cellule Ennemis de type "C"                               *
     *                                                                         *
     * Initialise les stats de la cellule chevalier                            *
     ***************************************************************************/
    chevalier->life = 100;
    chevalier->max_life = 100;
    chevalier->speed = 1;
}


void initVoleur(Ennemis *voleur){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - voleur : cellule Ennemis de type "V"                                  *
     *                                                                         *
     * Initialise les stats de la cellule voleur                               *
     ***************************************************************************/
    voleur->life = 85;
    voleur->max_life = 85;
    voleur->speed = 2;
}


void initPaladin(Ennemis *paladin){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - paladin : cellule Ennemis de type "P"                                 *
     *                                                                         *
     * Initialise les stats de la cellule paladin                              *
     ***************************************************************************/
    paladin->life = 350;
    paladin->max_life = 350;
    paladin->speed = 1;
}


void initMoine(Ennemis *moine){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - moine : cellule Ennemis de type "M"                                   *
     *                                                                         *
     * Initialise les stats de la cellule moine                                *
     ***************************************************************************/
    moine->life = 50;
    moine->max_life = 50;
    moine->speed = 1;
}


void initBerserker(Ennemis *berserker){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - berserker : cellule Ennemis de type "C"                               *
     *                                                                         *
     * Initialise les stats de la cellule berserker                            *
     ***************************************************************************/
    berserker->life = 260;
    berserker->max_life = 260;
    berserker->speed = 1;
}


Ennemis* alloueCellEnnemis(int turn, int line, char type){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - turn : tour d'apparition de l'ennemi                                  *
     * - line : ligne de l'ennemi                                              *
     * - type : type de l'ennemi                                               *
     *                                                                         *
     * - return Ennemis*                                                       *
     *                                                                         *
     * Crée une cellule Ennemis                                                *
     ***************************************************************************/
    Ennemis* tmp = (Ennemis*)malloc(sizeof(Ennemis));

    if (tmp != NULL){
        tmp->type = type;
        tmp->turn = turn;
        tmp->line = line;
        tmp->position = 26;
        tmp->next = NULL;
        tmp->next_line = NULL;
        tmp->prev_line = NULL;

        if (tmp->type == 'C'){
            initChevalier(tmp);
        }
        if (tmp->type == 'V'){
            initVoleur(tmp);
        }
        if (tmp->type == 'P'){
            initPaladin(tmp);
        }
        if (tmp->type == 'M'){
            initMoine(tmp);
        }
        if (tmp->type == 'B'){
            initBerserker(tmp);
        }

        return tmp;
    }
    return NULL;
}


void ajouteEnnemis(Ennemis* ennemis, Ennemis* tmp){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - ennemis : pointeur sur la struct Ennemis                              *
     * - tmp : cellule Ennemis                                                 *
     *                                                                         *
     * Ajoute une cellule Ennemis à la liste chainée Ennemis                   *
     ***************************************************************************/
    if (ennemis->next == NULL){
            ennemis->next = tmp;
        }
    else{
        while(ennemis->next != NULL){
            ennemis = ennemis->next;
        }
        ennemis->next = tmp;
    }
}


void initEnnemis(Ennemis *ennemis, char level[]){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - ennemis : pointeur sur la struct Ennemis                              *
     * - level : nom du fichier du niveau en cours                             *
     *                                                                         *
     * Initialise la liste chainée Ennemis à partir d'un fichier texte         *
     ***************************************************************************/
    ennemis->next = NULL;

    FILE* niv = fopen(level, "r");
    Ennemis* tmp;
    char type;
    int turn, ligne, money;
    fscanf(niv, " %d", &money);
    while(fscanf(niv, " %d %d %c", &turn, &ligne, &type) != EOF){
        do{
            tmp = alloueCellEnnemis(turn, ligne, type);
        }
        while(tmp == NULL);
        ajouteEnnemis(ennemis, tmp);
    }
    fclose(niv);
}


void initPointerLine(Ennemis* ennemis){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - ennemis : pointeur sur la struct ennemis                              *
     *                                                                         *
     * Réalise les chainages next_line et prev_line entre les différentes      *
     * cellules de la liste chainée Ennemis                                    *
     ***************************************************************************/
    Ennemis* PointerLine[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    while(ennemis->next != NULL){
        ennemis = ennemis->next;
        if (PointerLine[(ennemis->line)-1] != NULL){
            PointerLine[(ennemis->line)-1]->next_line = ennemis;
        }
        ennemis->prev_line = PointerLine[(ennemis->line)-1];
        PointerLine[(ennemis->line)-1] = ennemis;
    }
}


void supprEnnemis(Game* game){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct Game                                    *
     *                                                                         *
     * Supprime un ennemis quand scelui-ci est tué                             *
     ***************************************************************************/
    Ennemis* premier = game->ennemis;
    while(game->ennemis->next){
        Ennemis* enn_Prev = game->ennemis;
        game->ennemis = game->ennemis->next;
        if (game->ennemis->life <= 0){
            enn_Prev->next = game->ennemis->next;
            if (game->ennemis->next_line && game->ennemis->prev_line){
                game->ennemis->next_line->prev_line = game->ennemis->prev_line;
                game->ennemis->prev_line->next_line = game->ennemis->next_line;
            }
            else if(game->ennemis->prev_line){
                game->ennemis->prev_line->next_line = NULL;
            }
            else if (game->ennemis->next_line){
                game->ennemis->next_line->prev_line = NULL;
            }

            free(game->ennemis);
            game->ennemis = premier; 
        }
    }
    game->ennemis = premier;
}


/* 2) Struct defenseur */

void initArcher(Defenseur *archer){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - archer : cellule Defenseur de type "A"                                *
     *                                                                         *
     * Initialise les stats de la cellule archer                               *
     ***************************************************************************/
    archer->life = 3;
    archer->max_life = 3;
    archer->price = 150;
    archer->damage = 15;
}


void initSentinelle(Defenseur *sentinelle){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - sentinelle : cellule Defenseur de type "S"                            *
     *                                                                         *
     * Initialise les stats de la cellule sentinelle                           *
     ***************************************************************************/
    sentinelle->life = 9;
    sentinelle->max_life = 9;
    sentinelle->price = 75;
    sentinelle->damage = 10;
}


void initTrebuchet(Defenseur *trebuchet){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - trebuchet : cellule Defenseur de type "T"                             *
     *                                                                         *
     * Initialise les stats de la cellule trebuchet                            *
     ***************************************************************************/
    trebuchet->life = 1;
    trebuchet->max_life = 1;
    trebuchet->price = 250;
    trebuchet->damage = 40;
}

void initMage(Defenseur *mage){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - mage : cellule Defenseur de type "M"                                  *
     *                                                                         *
     * Initialise les stats de la cellule mage                                 *
     ***************************************************************************/
    mage->life = 3;
    mage->max_life = 3;
    mage->price = 300;
    mage->damage = 10;
}


void initOnagre(Defenseur *onagre){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - onagre : cellule Defenseur de type "O"                                *
     *                                                                         *
     * Initialise les stats de la cellule onagre                               *
     ***************************************************************************/
    onagre->life = 5;
    onagre->max_life = 5;
    onagre->price = 550;
    onagre->damage = 40;
}


Defenseur* alloueCellDefenseur(int line, int position, char type){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - position : colonne du defenseur                                       *
     * - line : ligne du defenseur                                             *
     * - type : type du defenseur                                              *
     *                                                                         *
     * - return Defenseur*                                                     *
     *                                                                         *
     * Crée une cellule Defenseur                                              *
     ***************************************************************************/
    Defenseur* tmp = (Defenseur*)malloc(sizeof(Defenseur));
  
    if (tmp != NULL){
        tmp->type = type;
        tmp->line = line;
        tmp->position = position;
        tmp->next = NULL;

        if (tmp->type == 'A'){
            initArcher(tmp);
        }
        if (tmp->type == 'S'){
            initSentinelle(tmp);
        }
        if (tmp->type == 'T'){
            initTrebuchet(tmp);
        }
        if (tmp->type == 'M'){
            initMage(tmp);
        }
        if (tmp->type == 'O'){
            initOnagre(tmp);
        }

        return tmp;
    }
    return NULL;
}


int testPosition(Defenseur* defenseur, int line, int position, int affichage){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - position : colonne du defenseur                                       *
     * - line : ligne du defenseur                                             *
     * - defenseur : pointeur sur la struct Defenseur                          *
     * - affichage : indique si l'on veut un affichage ascii                   *
     *                                                                         *
     * - return int                                                            *
     *                                                                         *
     * Vérifie s'il existe une tourelle aux coordonées ligne/position          *
     ***************************************************************************/
    Defenseur* premier = defenseur;
    
    if (line < 1 || line > 7){
        return 1;
    }
    if (position < 1 || position > 20){
        return 1;
    }
    if (defenseur->next){
        while(defenseur->next != NULL){
            defenseur = defenseur->next;
            if (defenseur->line == line && defenseur->position == position){
                if (affichage){
                    return 1;
                }
                printf("Soldat sur cette case\n");
                return 1;
            }
        }
    }
    defenseur = premier;
    return 0;
}


void choixPosition(Defenseur* defenseur, int *line, int *position){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - position : colonne du defenseur                                       *
     * - line : ligne du defenseur                                             *
     * - defenseur : pointeur sur la struct Defenseur                          *
     *                                                                         *
     * Choix de la position des défenseurs en Ascii                            *
     ***************************************************************************/
    do{
        printf("Où voulez vous le placer ?\n");
        printf("Ligne (1 à 7) : ");
        scanf(" %d", line);
        printf("Ligne (1 à 20) : ");
        scanf(" %d", position);
    }
    while (testPosition(defenseur, *line, *position, 0));
}


void ajouteDefenseur(Defenseur* defenseur, Defenseur* tmp){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - defenseur : pointeur sur la struct Defenseur                          *
     * - tmp : cellule de type  Defenseur*                                     *
     *                                                                         *
     * Ajoute une cellule défenseur à la liste chainée Defenseur               *
     ***************************************************************************/
    if (defenseur->next == NULL){
            defenseur->next = tmp;
        }
    else{
        while(defenseur->next != NULL){
            defenseur = defenseur->next;
        }
        defenseur->next = tmp;
    }
}


int verifSaisie(char choix, int money){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - choix : choix de la tourelle à poser                                  *
     * - money : argent du joueur                                              *
     *                                                                         *
     * Vérifie que la saisie du joueur est correcte                            *
     ***************************************************************************/
    if (choix != 'q' && choix != 'A' && choix != 'S' && choix != 'T' && choix != 'M' && choix != 'O'){
        printf("commande inconnue\n");
        return 1;
    }
    if ((choix == 'A' && money < 150) || (choix == 'S' && money < 75) || (choix == 'M' && money < 300) || 
        (choix == 'T' && money < 200) || (choix == 'O' && money < 550)){
        printf("Pas assez de pièces d'or\n");
        return 1;
    }
    return 0;
}


void afficheVagueAscii(Ennemis* ennemis){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - ennemis : cellule de type Ennemis*                                    *
     *                                                                         *
     * Affiche la vague d'ennemis en approche en Ascii                         *
     ***************************************************************************/
    if (ennemis->next == NULL){
        return;
    }
    Ennemis* premier = ennemis;
    int line, i, ligne = 1;
    printf("\nVoici la vague d'ennemis en approche\n");
    for (line = 1 ; line < 8 ; line ++){
        printf("%d|", line);
        ennemis = ennemis->next;
        while (ennemis->next && ennemis->line && ennemis->line != line){
            ennemis = ennemis->next;
        }
        
        while(ligne){
            if (ennemis->life < 100){
                printf("%d%c  ", ennemis->life, ennemis->type);
            }
            else {
                printf("%d%c ", ennemis->life, ennemis->type);
            }
            
            if (ennemis->next_line){
                for (i = 0 ; i < ennemis->next_line->turn - ennemis->turn - 1 ; i++){
                printf("     ");
                }
                ennemis = ennemis->next_line;
            }
            else{
                ligne = 0;
            }
        }
        
        printf("\n");
        ligne = 1;
        ennemis = premier;
    }
    printf("\n");
    ennemis = premier;
}


void barre_de_vie(int vie, int vie_max, int position, int line){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - vie : vie actuelle                                                    *
     * - vie_max : vie max                                                     *
     * - position : collonne sur laquelle est l'entité                         *
     * - line : ligne sur laquelle est l'entité                                *
     *                                                                         *
     * Affiche les barres de vie                                               *
     ***************************************************************************/
    float vieRestante = vie/(vie_max*1.0);

    MLV_draw_filled_rectangle(position+4, line-11, 42, 12, MLV_rgba(27, 29, 34, 255));
    MLV_draw_filled_rectangle(position+5, line-10, 40, 10, MLV_rgba(236, 30, 30, 255));
    MLV_draw_filled_rectangle(position+5, line-10, 40*vieRestante, 10, MLV_rgba(30, 236, 30, 255));
}


void afficheVagueGraphique(Ennemis *ennemis){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - ennemis : cellule de type Ennemis*                                    *
     *                                                                         *
     * Affiche la vague d'ennemis en approche sur l'interface graphique        *
     ***************************************************************************/
    int x, y, cmt = 1, afficheVague = 1, suiv, prec;
    MLV_Font *font = MLV_load_font("upheavtt.ttf", 50);
    MLV_Image *terrain = MLV_load_image("terrain.png");

    MLV_Image *chevalier = MLV_load_image("chevalier.png");
    MLV_Image *paladin = MLV_load_image("paladin.png");
    MLV_Image *voleur = MLV_load_image("voleur.png");
    MLV_Image *berserker = MLV_load_image("berserker.png");
    MLV_Image *moine = MLV_load_image("moine.png");

    if (ennemis->next == NULL){
        return;
    }
    else{
        while (afficheVague){
            MLV_clear_window(MLV_rgba(27, 29, 32, 255));
            MLV_draw_image(terrain, 0, 0);
            suiv = 0;
            prec = 0;
            Ennemis *premier = ennemis;
            while (ennemis->next){
                ennemis = ennemis->next;
                if (ennemis->turn <= 26*(cmt-1)){
                    MLV_draw_filled_rectangle(1410, 10, 60, 60, MLV_rgba(57, 59, 64, 255));
                    MLV_draw_filled_rectangle(1415, 15, 50, 50, MLV_rgba(182, 182, 182, 255));
                    MLV_draw_text_with_font(1430, 15, "<", font, MLV_rgba(0, 9, 3, 255));
                    prec = 1;
                }
                if (ennemis->turn >= 26*(cmt-1)){
                    if (ennemis->type == 'C'){
                        MLV_draw_image(chevalier, TAILLE_MARGE_X + (ennemis->turn - 26*(cmt-1)) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                    }
                    else if (ennemis->type == 'P'){
                        MLV_draw_image(paladin, TAILLE_MARGE_X + (ennemis->turn - 26*(cmt-1)) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                    }
                    else if (ennemis->type == 'V'){
                        MLV_draw_image(voleur, TAILLE_MARGE_X + (ennemis->turn - 26*(cmt-1)) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                    }
                    else if (ennemis->type == 'B'){
                        MLV_draw_image(berserker, TAILLE_MARGE_X + (ennemis->turn - 26*(cmt-1)) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                    }
                    else if (ennemis->type == 'M'){
                        MLV_draw_image(moine, TAILLE_MARGE_X + (ennemis->turn - 26*(cmt-1)) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                    }
                }
            }
            
            if (ennemis->turn >= 26*cmt){
                MLV_draw_filled_rectangle(1480, 10, 60, 60, MLV_rgba(57, 59, 64, 255));
                MLV_draw_filled_rectangle(1485, 15, 50, 50, MLV_rgba(182, 182, 182, 255));
                MLV_draw_text_with_font(1500, 15, ">", font, MLV_rgba(0, 9, 3, 255));
                suiv = 1;
            }

            MLV_draw_filled_rectangle(1550, 10, 200, 60, MLV_rgba(57, 59, 64, 255));
            MLV_draw_filled_rectangle(1555, 15, 190, 50, MLV_rgba(182, 182, 182, 255));
            MLV_draw_text_with_font(1560, 15, "Retour", font, MLV_rgba(0, 9, 3, 255));


            ennemis = premier;
            MLV_actualise_window();
            MLV_wait_mouse(&x, &y);
            if (x >= 1480 && x <= 1540 && y >= 10 && y <= 70 && suiv){
                cmt ++;
            }
            else if (x >= 1410 && x <= 1470 && y >= 10 && y <= 70 && prec){
                cmt --;
            }
            else if (x >= 1550 && x <= 1750 && y >= 10 && y <= 70){
                afficheVague = 0;
            }
        }
    }
}



void choixDefenseAscii(Game* game, Defenseur* defenseur, Ennemis* ennemis){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la structure game                                 *
     * - ennemis : cellule de type Ennemis*                                    *
     * - defenseur : pointeur sur la struct defenseur                          *
     *                                                                         *
     * Affiche les choix de défense en Ascii et permet au joueur de les poser  *
     ***************************************************************************/
    int line, position;
    Defenseur* tmp;
    afficheVagueAscii(ennemis);
    while(1){
        char choix = '\0';
        do {
            printf("Quel défenseur voulez-vous engager ? Vous avez %d po\n", game->money);
            printf("A : Archer (150 po)\n");
            printf("S : Sentinelle (75 po)\n");
            printf("T : Trébuchet (250 po)\n");
            printf("M : Mage (300 po)\n");
            printf("O : Onagre (550 po)\n");
            printf("q : Début du tour\n");
            scanf(" %c", &choix);
        }
        while (verifSaisie(choix, game->money));

        if (choix == 'q'){
            return;
        }

        choixPosition(defenseur, &line, &position);
        do{
            tmp = alloueCellDefenseur(line, position, choix);
        }
        while(tmp == NULL);
        
        game->money -= tmp->price;
        ajouteDefenseur(defenseur, tmp);
        printf("\n");
    }
}


void afficheTourelleGraphique(Defenseur *defenseur){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - defenseur : pointeur sur la struct defenseur                          *
     *                                                                         *
     * Affiche les tourelles sur l'interface graphique                         *
     ***************************************************************************/
    MLV_Image *archer = MLV_load_image("archer.png");
    MLV_Image *sentinelle = MLV_load_image("sentinelle.png");
    MLV_Image *mage = MLV_load_image("mage.png");
    MLV_Image *trebuchet = MLV_load_image("trebuchet.png");
    MLV_Image *onagre = MLV_load_image("onagre.png");
    if (defenseur->next == NULL){
        return;
    }
    else{
        Defenseur *premier = defenseur;
        while (defenseur->next){
            defenseur = defenseur->next;
            if (defenseur->type == 'A'){
                MLV_draw_image(archer, TAILLE_MARGE_X + (defenseur->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (defenseur->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                barre_de_vie(defenseur->life, defenseur->max_life, TAILLE_MARGE_X + (defenseur->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (defenseur->line - 1) * (TAILLE_CASE + ESPACE_CASE));
            }
            else if (defenseur->type == 'S'){
                MLV_draw_image(sentinelle, TAILLE_MARGE_X + (defenseur->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (defenseur->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                barre_de_vie(defenseur->life, defenseur->max_life, TAILLE_MARGE_X + (defenseur->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (defenseur->line - 1) * (TAILLE_CASE + ESPACE_CASE));
            }
            else if (defenseur->type == 'M'){
                MLV_draw_image(mage, TAILLE_MARGE_X + (defenseur->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (defenseur->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                barre_de_vie(defenseur->life, defenseur->max_life, TAILLE_MARGE_X + (defenseur->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (defenseur->line - 1) * (TAILLE_CASE + ESPACE_CASE));
            }
            else if (defenseur->type == 'T'){
                MLV_draw_image(trebuchet, TAILLE_MARGE_X + (defenseur->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (defenseur->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                barre_de_vie(defenseur->life, defenseur->max_life, TAILLE_MARGE_X + (defenseur->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (defenseur->line - 1) * (TAILLE_CASE + ESPACE_CASE));
            }
            else if (defenseur->type == 'O'){
                MLV_draw_image(onagre, TAILLE_MARGE_X + (defenseur->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (defenseur->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                barre_de_vie(defenseur->life, defenseur->max_life, TAILLE_MARGE_X + (defenseur->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (defenseur->line - 1) * (TAILLE_CASE + ESPACE_CASE));
            }
        }
        MLV_actualise_window();
        defenseur = premier;
    }
}


void positionDefenseGraphique(Defenseur *defenseur){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - defenseur : pointeur sur la struct defenseur                          *
     *                                                                         *
     * Affiche les endroits où il est possible de poser une défense            *
     ***************************************************************************/
    int i, j;
    for (i = 0; i < 7; i++){
        for (j = 0; j < 20; j++){
            if (testPosition(defenseur, i+1, j+1, 1) == 0){
                MLV_draw_filled_circle(TAILLE_MARGE_X + 30 + j * TAILLE_CASE, TAILLE_MARGE_Y + 30 + i * TAILLE_CASE + i * ESPACE_CASE, 10, MLV_rgba(150, 99, 18, 255));
            }
        }
    }
    MLV_actualise_window();
}


void choixPositionGraphique(int *line, int *position, Defenseur *defenseur){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - defenseur : pointeur sur la struct defenseur                          *
     * - line : ligne sur laquelle est le defenseur                            *
     * - position : colonne sur laquelle est le defenseur                      *
     *                                                                         *
     * Vérifie que le joueur clique bien sur un emplacement valide             *
     ***************************************************************************/
    int x, y, i, j;
    while(testPosition(defenseur, *line, *position, 1)){
        MLV_wait_mouse(&x, &y);
        for (i = 0 ; i < 7 ; i++){
            for (j = 0 ; j < 20 ; j++){
                if (x > TAILLE_MARGE_X + j * TAILLE_CASE && x < TAILLE_MARGE_X + (j + 1) * TAILLE_CASE
                    && y > TAILLE_MARGE_Y + i * TAILLE_CASE + i * ESPACE_CASE && y < TAILLE_MARGE_Y + (i + 1) * TAILLE_CASE + i * ESPACE_CASE){
                        *line = i + 1;
                        *position = j + 1;
                }
            }
        }
    }
}


void choixDefenseGraphique(Game *game, Defenseur *defenseur, Ennemis *ennemis){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la structure game                                 *
     * - ennemis : cellule de type Ennemis*                                    *
     * - defenseur : pointeur sur la struct defenseur                          *
     *                                                                         *
     * Affiche l'interface dans laquelle le joueur choisie ses défenses        *
     ***************************************************************************/
    int x = 0, y = 0;
    Defenseur* tmp;
    while (x < 1580 || x > 1770 || y < 10 || y > 100){

        char choix = '\0';
        int line = 0, position = 0;

        /* Initialisation des éléments du décors */
        //Police
        MLV_Font* font = MLV_load_font("upheavtt.ttf", 24);
        MLV_Font* font_start = MLV_load_font("upheavtt.ttf", 36);

        //Image
        MLV_Image *terrain = MLV_load_image("terrain.png");
        MLV_Image *archer = MLV_load_image("archer.png");
        MLV_Image *sentinelle = MLV_load_image("sentinelle.png");
        MLV_Image *mage = MLV_load_image("mage.png");
        MLV_Image *trebuchet = MLV_load_image("trebuchet.png");
        MLV_Image *onagre = MLV_load_image("onagre.png");
        MLV_Image *piece_or = MLV_load_image("po.png");

        MLV_clear_window(MLV_rgba(27, 29, 32, 255));
        MLV_draw_image(terrain, 0, 0);

        /* Bouton Start/Affiche vague */
        MLV_draw_filled_rectangle(1580, 10, 190, 90, MLV_rgba(57, 59, 64, 255));
        MLV_draw_filled_rectangle(1585, 15, 180, 80, MLV_rgba(182, 182, 182, 255));
        MLV_draw_text_with_font(1610, 35, "Start !", font_start, MLV_rgba(0, 9, 3, 255));

        MLV_draw_filled_rectangle(1190, 10, 370, 90, MLV_rgba(57, 59, 64, 255));
        MLV_draw_filled_rectangle(1195, 15, 360, 80, MLV_rgba(182, 182, 182, 255));
        MLV_draw_text_with_font(1200, 35, "Afficher la vague", font_start, MLV_rgba(0, 9, 3, 255));

        /* Affichage des choix de défense */
        MLV_draw_filled_rectangle(300, 0, 760, 125, MLV_rgba(47, 49, 52, 155));

        MLV_draw_image(archer, 340, 20);
        MLV_draw_text_with_font(322, 75, "Archer", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_text_with_font(347, 95, "150", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_image(piece_or, 385, 97);

        MLV_draw_image(sentinelle, 460, 20);
        MLV_draw_text_with_font(415, 75, "Sentinelle", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_text_with_font(463, 95, "75", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_image(piece_or, 502, 97);

        MLV_draw_image(mage, 580, 20);
        MLV_draw_text_with_font(575, 75, "Mage", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_text_with_font(580, 95, "300", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_image(piece_or, 627, 97);

        MLV_draw_image(trebuchet, 700, 20);
        MLV_draw_text_with_font(664, 75, "Trébuchet", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_text_with_font(705, 95, "250", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_image(piece_or, 752, 97);

        MLV_draw_image(onagre, 820, 20);
        MLV_draw_text_with_font(805, 75, "Onagre", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_text_with_font(823, 95, "550", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_image(piece_or, 870, 97);

        /* Information sur l'argent du Joueur */
        MLV_draw_line(910, 10, 910, 115, MLV_rgba(255, 255, 255, 255));
        MLV_draw_text_with_font(935, 35, "Argent :", font, MLV_rgba(255, 255, 255, 255));
        MLV_draw_text_with_font(945, 65, "%d", font, MLV_rgba(255, 255, 255, 255), game->money);
        MLV_draw_image(piece_or, 1000, 67);

        afficheTourelleGraphique(defenseur);

        MLV_actualise_window();

        MLV_wait_mouse(&x, &y);
        
        if (x >= 340 && x <= 390 && y >= 20 && y <= 70 && game->money >= 150){
            choix = 'A';
            positionDefenseGraphique(defenseur);
            afficheTourelleGraphique(defenseur);
            choixPositionGraphique(&line, &position, defenseur);
        }
        else if (x >= 460 && x <= 510 && y >= 20 && y <= 70 && game->money >= 75){
            choix = 'S';
            positionDefenseGraphique(defenseur);
            afficheTourelleGraphique(defenseur);
            choixPositionGraphique(&line, &position, defenseur);
        }
        else if (x >= 580 && x <= 630 && y >= 20 && y <= 70 && game->money >= 300){
            choix = 'M';
            positionDefenseGraphique(defenseur);
            afficheTourelleGraphique(defenseur);
            choixPositionGraphique(&line, &position, defenseur);
        }
        else if (x >= 700 && x <= 750 && y >= 20 && y <= 70 && game->money >= 250){
            choix = 'T';
            positionDefenseGraphique(defenseur);
            afficheTourelleGraphique(defenseur);
            choixPositionGraphique(&line, &position, defenseur);
        }
        else if (x >= 820 && x <= 870 && y >= 20 && y <= 70 && game->money >= 550){
            choix = 'O';
            positionDefenseGraphique(defenseur);
            afficheTourelleGraphique(defenseur);
            choixPositionGraphique(&line, &position, defenseur);
        }
        else if (x >= 1190 && x <= 1560 && y >= 10 && y <= 100){
            afficheVagueGraphique(ennemis);
        }
        if (line != 0 && position != 0){
            do{
                tmp = alloueCellDefenseur(line, position, choix);
            }
            while(tmp == NULL);
        game->money -= tmp->price;
        ajouteDefenseur(defenseur, tmp);
        }
    }
}


void supprDefenseur(Game* game){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la structure game                                 *
     *                                                                         *
     * Supprime les tourelles mortes de la liste chainée Defenseur             *
     ***************************************************************************/
    Defenseur* premier = game->defenseur;
    if (game->defenseur){
        while (game->defenseur->next){
            Defenseur* def_Prev = game->defenseur;
            game->defenseur = game->defenseur->next;
            if (game->defenseur->life <= 0){
                def_Prev->next = game->defenseur->next;
                free(game->defenseur);
                game->defenseur = premier;
            }
        }
    }
    game->defenseur = premier;
}


/* 3) Struct game */
void initGame(Game *game, Ennemis *ennemis, Defenseur *defenseur, char level[], int affichage){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - ennemis : pointeur sur la struct ennemis                              *
     * - defenseur : pointeur sur la struct defenseur                          *
     * - level : nom du fichier contenant les informations du  niveau en cours *
     * - affichage : variable determinant le mode d'affichage                  *
     *                                                                         *
     * Initialise la structure game                                            *
     ***************************************************************************/
    /* Initialisation des ennemis */
    initEnnemis(ennemis, level);
    initPointerLine(ennemis);

    /* Initialistaion de la struct game */
    FILE* niv = fopen(level, "r");
    int money;
    fscanf(niv, " %d", &money);
    fclose(niv);
    game->money += money;
    game->turn = 0;

    /* Modification de la struct defenseur */
    if (affichage){
        choixDefenseGraphique(game, defenseur, ennemis);
    }
    else{
        choixDefenseAscii(game, defenseur, ennemis);
    }

    /* Liaison entre les structs defenseur/ ennemis et game */
    game->ennemis = ennemis;
    game->defenseur = defenseur;
}


/* Interaction defenseur/ ennemis */
int collisionEnnDef(Game* game, Ennemis* ennemis){
    Defenseur* premier = game->defenseur;
    int bool = 1;
    if (game->defenseur){
        while (game->defenseur->next){
            game->defenseur = game->defenseur->next;
            if (ennemis->speed == 2){
                if (ennemis->position-1 == game->defenseur->position && ennemis->line == game->defenseur->line){
                    bool = 0;
                }
                if (ennemis->position-2 == game->defenseur->position && ennemis->line == game->defenseur->line && bool != 0){
                    bool = 2;
                }
            }
            if (ennemis->position-1 == game->defenseur->position && ennemis->line == game->defenseur->line){
                bool = 0;
            }
        }
    }
    game->defenseur = premier;
    return bool;
}


int collisionEnnEnn(Ennemis* ennemis){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - ennemis : cellule de type Ennemis*                                    *
     *                                                                         *
     * Gère les collisions entre ennemis                                       *
     ***************************************************************************/
    if ((ennemis->prev_line && ennemis->position-1 == ennemis->prev_line->position && ennemis->line == ennemis->prev_line->line) 
         || (ennemis->prev_line && ennemis->prev_line->position == ennemis->position)){
        return 0;
    }
    if (ennemis->speed == 2 && ennemis->prev_line && ennemis->position-2 == ennemis->prev_line->position && ennemis->line == ennemis->prev_line->line){
        return 2;
    }
    return 1;
}


void attaqueArcher(Game* game, Defenseur* archer){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - archer : cellule de type Defenseur*                                   *
     *                                                                         *
     * Fais attaquer les archers                                               *
     ***************************************************************************/
    Ennemis* premierEnn = game->ennemis;
    while(game->ennemis->next && game->ennemis->line != archer->line){
        game->ennemis = game->ennemis->next;
    }
    if (game->ennemis->line == archer->line){
        while(game->ennemis->prev_line){
            game->ennemis = game->ennemis->prev_line;
        }
        if (game->ennemis && game->ennemis->position < 26){
            game->ennemis->life -= archer->damage;
        }
    }
    game->ennemis = premierEnn;
}


void attaqueSentinelle(Game* game, Defenseur* sentinelle){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - sentinelle : cellule de type Defenseur*                               *
     *                                                                         *
     * Fais attaquer les sentinelles                                           *
     ***************************************************************************/
    Ennemis* premierEnn = game->ennemis;
    while(game->ennemis->next && game->ennemis->line != sentinelle->line){
        game->ennemis = game->ennemis->next;
    }
    if (game->ennemis->line == sentinelle->line){
        while(game->ennemis->prev_line){
            game->ennemis = game->ennemis->prev_line;
        }
        if (sentinelle->position+1 == game->ennemis->position){
            game->ennemis->life -= sentinelle->damage;
        }
    }
    game->ennemis = premierEnn;
}


void attaqueTrebuchet(Game* game, Defenseur* trebuchet){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - trébuchet : cellule de type Defenseur*                                *
     *                                                                         *
     * Fais attaquer les trébuchets                                            *
     ***************************************************************************/
    Ennemis* premierEnn = game->ennemis;
    while(game->ennemis->next && game->ennemis->line != trebuchet->line){
        game->ennemis = game->ennemis->next;
    }
    if (game->ennemis->line == trebuchet->line){
        while(game->ennemis->next_line && game->ennemis->next_line->position < 26){
            game->ennemis = game->ennemis->next_line;
        }
        if (game->turn%2 == 0){
            game->ennemis->life -= trebuchet->damage;
        }
    }
    game->ennemis = premierEnn;
}


void attaqueMage(Game* game, Defenseur* mage){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - mage : cellule de type Defenseur*                                     *
     *                                                                         *
     * Fais attaquer les mages                                                 *
     ***************************************************************************/
    Ennemis* premierEnn = game->ennemis;
    while(game->ennemis->next && game->ennemis->line != mage->line){
        game->ennemis = game->ennemis->next;
    }
    if (game->ennemis->line == mage->line){
        while(game->ennemis->prev_line){
            game->ennemis = game->ennemis->prev_line;
        }
        int posEnnemis = game->ennemis->position;
        game->ennemis->life -= mage->damage;
        while(game->ennemis->next_line && game->ennemis->next_line->position <= posEnnemis + 3 && game->ennemis->next_line->position < 26){
            game->ennemis = game->ennemis->next_line;
            game->ennemis->life -= mage->damage;
        }
    }
    game->ennemis = premierEnn;
}


void attaqueOnagre(Game* game, Defenseur* onagre){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - onagre : cellule de type Defenseur*                                   *
     *                                                                         *
     * Fais attaquer les onagres                                               *
     ***************************************************************************/
    Ennemis* premierEnn = game->ennemis;
    while(game->ennemis->next && game->ennemis->line != onagre->line){
        game->ennemis = game->ennemis->next;
    }
    if (game->ennemis->line == onagre->line){
        while(game->ennemis->prev_line){
            game->ennemis = game->ennemis->prev_line;
        }

        int abscisseTir = game->ennemis->position + 1, ordonneeTir = game->ennemis->line;
        if (game->ennemis->position < 26){
            game->ennemis->life -= onagre->damage;
        }
        game->ennemis = premierEnn;
        while(game->ennemis->next){
            game->ennemis = game->ennemis->next;
            if (game->ennemis->position < 26){
                if ((game->ennemis->position == abscisseTir && game->ennemis->line == ordonneeTir) ||
                    (game->ennemis->position == abscisseTir && game->ennemis->line == ordonneeTir - 1) ||
                    (game->ennemis->position == abscisseTir && game->ennemis->line == ordonneeTir + 1) ||
                    (game->ennemis->position == abscisseTir + 1 && game->ennemis->line == ordonneeTir)){
                    game->ennemis->life -= onagre->damage;
                }
            }
        }
    }
    game->ennemis = premierEnn;
}


void attaqueDefenseur(Game* game){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     *                                                                         *
     * Fais attaquer les defenseurs                                            *
     ***************************************************************************/
    Defenseur* premierDef = game->defenseur;
    if (game->ennemis && game->defenseur){
        while(game->defenseur->next){
            game->defenseur = game->defenseur->next;
            if (game->defenseur->type == 'A'){
                attaqueArcher(game, game->defenseur);
            }
            if (game->defenseur->type == 'S'){
                attaqueSentinelle(game, game->defenseur);
            }
            if (game->defenseur->type == 'T'){
                attaqueTrebuchet(game, game->defenseur);
            }
            if (game->defenseur->type == 'M'){
                attaqueMage(game, game->defenseur);
            }
            if (game->defenseur->type == 'O'){
                attaqueOnagre(game, game->defenseur);
            }
        }
    }
    game->defenseur= premierDef;
}   


void attaqueEnnemis(Game* game){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     *                                                                         *
     * Fais attaquer les ennemis                                               *
     ***************************************************************************/
    Ennemis* premierEnn = game->ennemis;
    Defenseur* premierDef = game->defenseur;
    if (game->ennemis && game->defenseur){
        while(game->ennemis->next){
            game->ennemis = game->ennemis->next;
            while (game->defenseur->next){
                game->defenseur = game->defenseur->next;
                if (game->defenseur->position == game->ennemis->position-1 && game->ennemis->line == game->defenseur->line){
                    if (game->ennemis->type == 'B' && game->ennemis->life <= 125){
                        game->defenseur->life -= 2;
                    }
                    else{
                        game->defenseur->life --;
                    }
                }
            }
            game->defenseur= premierDef;
        }
    }
    game->ennemis = premierEnn;
    game->defenseur= premierDef;
}   



/* AFFICHAGE ASCII */
void initPlateau(Plateau *plateau){
    int i, j;
    for (i = 0 ; i < 7 ; i++){
        for (j = 0 ; j < 26 ; j++){
            plateau->ennemis[i][j] = NULL;
            plateau->tourelles[i][j] = NULL;
        }
    }
}


void avanceChevalier(Game *game, Ennemis *chevalier){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - ennemis : cellule de type Ennemis*                                    *
     *                                                                         *
     * Fais avancer les chevalier                                              *
     ***************************************************************************/
    if (chevalier->turn <= game->turn && chevalier->position >= 0 && 
        collisionEnnDef(game, chevalier) && collisionEnnEnn(chevalier)){
        chevalier->position --;
    }
}


void avanceMoine(Game *game, Ennemis *moine){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - moine : cellule de type Ennemis*                                      *
     *                                                                         *
     * Fais avancer les moines                                                 *
     ***************************************************************************/
    if (moine->turn <= game->turn && moine->position >= 0 && 
        collisionEnnDef(game, moine) && collisionEnnEnn(moine)){
        moine->position --;
    }
}


void avancePaladin(Game *game, Ennemis *paladin){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - paladin : cellule de type Ennemis*                                    *
     *                                                                         *
     * Fais avancer les paladins                                               *
     ***************************************************************************/
    if (paladin->turn <= game->turn && paladin->position >= 0 && game->turn % 2 &&
        collisionEnnDef(game, paladin) && collisionEnnEnn(paladin)){
        paladin->position --;
    }
}


void avanceVoleur(Game *game, Ennemis *voleur){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - voleur : cellule de type Ennemis*                                     *
     *                                                                         *
     * Fais avancer les voleurs                                                *
     ***************************************************************************/
    if (voleur->turn <= game->turn && voleur->position >= 0 && 
        collisionEnnDef(game, voleur) == 1 && collisionEnnEnn(voleur) == 1){
        voleur->position -= 2;
    }
    else if (voleur->turn <= game->turn && voleur->position >= 0 && 
            (collisionEnnDef(game, voleur) == 2  || collisionEnnEnn(voleur) == 2)){
        voleur->position --;
    }
}


void avanceBerserker(Game *game, Ennemis *berserker){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - berserker : cellule de type Ennemis*                                  *
     *                                                                         *
     * Fais avancer les berserkers                                             *
     ***************************************************************************/
    if (berserker->life <= 125){
        berserker->speed = 2;
    }
    if (berserker->speed == 1){
        if (berserker->turn <= game->turn && berserker->position >= 0 && 
            collisionEnnDef(game, berserker) && collisionEnnEnn(berserker)){
            berserker->position --;
        }
    }
    else {
        if (berserker->turn <= game->turn && berserker->position >= 0 && 
            collisionEnnDef(game, berserker) == 1 && collisionEnnEnn(berserker) == 1){
            berserker->position -= 2;
        }
        else if (berserker->turn <= game->turn && berserker->position >= 0 && 
                (collisionEnnDef(game, berserker) == 2  || collisionEnnEnn(berserker) == 2)){
            berserker->position --;
        }
    }
}


void tourEnnemis(Game *game, Ennemis *ennemis){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - ennemis : cellule de type Ennemis*                                    *
     *                                                                         *
     * Fais avancer les ennemis                                                *
     ***************************************************************************/
    if (ennemis->type == 'C'){
        avanceChevalier(game, ennemis);
    }
    if (ennemis->type == 'V'){
        avanceVoleur(game, ennemis);
    }
    if (ennemis->type == 'P'){
        avancePaladin(game, ennemis);
    }
    if (ennemis->type == 'B'){
        avanceBerserker(game, ennemis);
    }
    if (ennemis->type == 'M'){
        avanceMoine(game, ennemis);
    }
}


void effetMoine(Game *game){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     *                                                                         *
     * Active l'effet de soin du moine                                         *
     ***************************************************************************/
    Ennemis* premier = game->ennemis;
    while (game->ennemis->next){
        game->ennemis = game->ennemis->next;
        if (game->ennemis->type == 'M' && game->ennemis->position < 26){
            if (game->ennemis->prev_line){
                if (game->ennemis->prev_line->life + 8 <= game->ennemis->prev_line->max_life){
                    game->ennemis->prev_line->life += 8;
                }
                if (game->ennemis->prev_line->prev_line){
                    if (game->ennemis->prev_line->prev_line->life + 8 <= game->ennemis->prev_line->prev_line->max_life){
                        game->ennemis->prev_line->prev_line->life += 8;
                    }
                }
            }
            if (game->ennemis->next_line){
                if (game->ennemis->next_line->life + 8 <= game->ennemis->next_line->max_life){
                    game->ennemis->next_line->life += 8;
                }
                if (game->ennemis->next_line->next_line){
                    if (game->ennemis->next_line->next_line->life + 8 <= game->ennemis->next_line->next_line->max_life){
                        game->ennemis->next_line->next_line->life += 8;
                    }
                }
            }

            Ennemis* premierBis = premier;
            while (premierBis->next){
                premierBis = premierBis->next;
                if (premierBis->position == game->ennemis->position && premierBis->life+8 <= premierBis->max_life){
                    premierBis->life += 8;
                }
            }
        }
    }
    game->ennemis = premier;
}


void actualisePlateauEnnemis(Plateau *plateau, Ennemis *ennemis){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - plateau : struct contenant des informations sur les positions des     *
     *             ennemis et des défenseurs                                   *
     * - ennemis : cellule ennemis                                             *
     *                                                                         *
     * Actualise les positions des ennemis dans la liste ennemis de la struct  *
     * plateau                                                                 *
     ***************************************************************************/
    if (ennemis->position && ennemis->position < 26){
        plateau->ennemis[(ennemis->line)-1][ennemis->position] = ennemis;
    }
}


void actualisePlateauDefenseur(Plateau *plateau, Defenseur *defenseur){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - plateau : struct contenant des informations sur les positions des     *
     *             ennemis et des défenseurs                                   *
     * - defenseur : cellule Defenseur                                         *
     *                                                                         *
     * Supprime les entités mortes, fait avancer les ennemis et active leurs   *
     * effets                                                                  *
     ***************************************************************************/
    plateau->tourelles[(defenseur->line)-1][defenseur->position] = defenseur;
}


void actualiseJeu(Game *game, Plateau *plateau){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     * - plateau : struct contenant des informations sur les positions des     *
     *             ennemis et des défenseurs                                   *
     *                                                                         *
     * Supprime les entités mortes, fait avancer les ennemis et active leurs   *
     * effets                                                                  *
     ***************************************************************************/
    initPlateau(plateau);
    Ennemis* premierEnn = game->ennemis;
    Defenseur* premierDef = game->defenseur;
    game->turn ++;
    supprDefenseur(game);
    supprEnnemis(game);
    if (game->defenseur){
        while (game->defenseur->next != NULL){
            game->defenseur = game->defenseur->next;
            actualisePlateauDefenseur(plateau, game->defenseur);
        }
    }
    game->defenseur = premierDef;
    while (game->ennemis->next != NULL){
        game->ennemis = game->ennemis->next;
        tourEnnemis(game, game->ennemis);
        actualisePlateauEnnemis(plateau, game->ennemis);
    }
    game->ennemis = premierEnn;
    effetMoine(game);
}


void afficheJeuAscii(Plateau plateau){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - plateau : struct contenant des informations sur les positions des     *
     *             ennemis et des défenseurs                                   *
     *                                                                         *
     * Affiche la vague ainsi que les tourelles lors du jeu en Ascii           *
     ***************************************************************************/
    int i, j;
    for (i = 0 ; i < 7 ; i++){
        printf("%d| ", i + 1);
        for (j = 0 ; j < 26 ; j++){
            if (plateau.ennemis[i][j] && plateau.ennemis[i][j]->position > 0 && plateau.ennemis[i][j]->position < 26 && plateau.ennemis[i][j]->life < 10){
                printf("%d%c   ", plateau.ennemis[i][j]->life, plateau.ennemis[i][j]->type);
            }
            else if (plateau.ennemis[i][j] && plateau.ennemis[i][j]->position > 0 && plateau.ennemis[i][j]->position < 26 && plateau.ennemis[i][j]->life < 100){
                printf("%d%c  ", plateau.ennemis[i][j]->life, plateau.ennemis[i][j]->type);
            }
            else if (plateau.ennemis[i][j] && plateau.ennemis[i][j]->position > 0 && plateau.ennemis[i][j]->position < 26){
                printf("%d%c ", plateau.ennemis[i][j]->life, plateau.ennemis[i][j]->type);
            }
           
            else if(plateau.tourelles[i][j]){
                printf("%d%c   ", plateau.tourelles[i][j]->life, plateau.tourelles[i][j]->type);
            }
            else{
                printf("     ");
            }
        }
        printf("\n");
    }
}


void afficheEnnemisGraphique(Ennemis *ennemis){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - ennemis : pointeur sur la struct Ennemis                              *
     *                                                                         *
     * Affiche les ennemis sur l'interface graphique                           *
     ***************************************************************************/
    MLV_Image *chevalier = MLV_load_image("chevalier.png");
    MLV_Image *paladin = MLV_load_image("paladin.png");
    MLV_Image *voleur = MLV_load_image("voleur.png");
    MLV_Image *berserker = MLV_load_image("berserker.png");
    MLV_Image *moine = MLV_load_image("moine.png");

     if (ennemis->next == NULL){
        return;
    }
    else{
        Ennemis *premier = ennemis;
        while (ennemis->next){
            ennemis = ennemis->next;
            if (ennemis->position < 26){
                if (ennemis->type == 'C'){
                    MLV_draw_image(chevalier, TAILLE_MARGE_X + (ennemis->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                    barre_de_vie(ennemis->life, ennemis->max_life, TAILLE_MARGE_X + (ennemis->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                }
                else if (ennemis->type == 'P'){
                    MLV_draw_image(paladin, TAILLE_MARGE_X + (ennemis->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                    barre_de_vie(ennemis->life, ennemis->max_life, TAILLE_MARGE_X + (ennemis->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                }
                else if (ennemis->type == 'V'){
                    MLV_draw_image(voleur, TAILLE_MARGE_X + (ennemis->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                    barre_de_vie(ennemis->life, ennemis->max_life, TAILLE_MARGE_X + (ennemis->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                }
                else if (ennemis->type == 'B'){
                    MLV_draw_image(berserker, TAILLE_MARGE_X + (ennemis->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                    barre_de_vie(ennemis->life, ennemis->max_life, TAILLE_MARGE_X + (ennemis->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                }
                else if (ennemis->type == 'M'){
                    MLV_draw_image(moine, TAILLE_MARGE_X + (ennemis->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                    barre_de_vie(ennemis->life, ennemis->max_life, TAILLE_MARGE_X + (ennemis->position - 1) * TAILLE_CASE, TAILLE_MARGE_Y + (ennemis->line - 1) * (TAILLE_CASE + ESPACE_CASE));
                }
            }
        }
        MLV_actualise_window();
        ennemis = premier;
    }
}


void afficheJeuGraphique(Game *game){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     *                                                                         *
     * - return int                                                            *
     *                                                                         *
     * Regle la partie avec les modes de jeu choisi                            *
     ***************************************************************************/
        MLV_Image *terrain = MLV_load_image("terrain.png");

        MLV_clear_window(MLV_rgba(27, 29, 32, 255));
        MLV_draw_image(terrain, 0, 0);

        afficheTourelleGraphique(game->defenseur);
        afficheEnnemisGraphique(game->ennemis);

        MLV_actualise_window();
}



int condFinPartie(Game *game){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - game : pointeur sur la struct game                                    *
     *                                                                         *
     * - return int                                                            *
     *                                                                         *
     * Vérifie si la partie est finie ou non                                   *
     ***************************************************************************/
    if (game->ennemis->next == NULL){
        return 2;
    }
    Ennemis* premier = game->ennemis;
    while (game->ennemis->next){
        game->ennemis = game->ennemis->next;
        if (game->ennemis->position && game->ennemis->position <= 1){
            return 0;
        }
    }
    game->ennemis = premier;
    return 1;
}


int mode(int argc, char *argv[], int *affichage){
    /***************************************************************************
     * PARAMETRE :                                                             *
     * - argc : nombre d'arguments fournis                                     *
     * - argv : liste des arguments fournis                                    *
     * - affichage : mode d'affichage                                          *
     *                                                                         *
     * - return int                                                            *
     *                                                                         *
     * Regle la partie avec le mode de jeu choisi                              *
     ***************************************************************************/
    if(argc>2){
        printf("-a pour la version Ascii OU -g pour la version Graphique\n"); 
        return 0;
    }
    if(argc<2){
        printf("-a pour la version Ascii OU -g pour la version Graphique\n"); 
        return 0;
        }
    
    for(int i=1; i<argc; i++){
        int ret = strncmp(argv[i], "-g", 2);
        if(ret == 0){
            *affichage = 1;
        }
        ret = strncmp(argv[i], "-a", 2);
        if(ret == 0){
            *affichage = 0;
        }
    }
    if(*affichage != -1){
        return 1;
    }
    printf("-a pour la version Ascii OU -g pour la version Graphique\n");
    return 0;
}


int main(int argc, char *argv[]){
    Game game;
    game.money = 0;
    Ennemis ennemis;
    Defenseur defenseur;
    defenseur.next = NULL;
    Plateau plateau;
    FILE* levels = fopen("levels.txt", "r");
    char niv[20];
    int level = 2, affichage = -1;

    if (mode(argc, argv, &affichage)){
        if (affichage){
            MLV_create_window("Castle Defense", "Castle Defense", TAILLE_FENETRE_X, TAILLE_FENETRE_Y);
        }

        while (fscanf(levels, " %s", niv) != EOF && level){
            initGame(&game, &ennemis, &defenseur, niv, affichage);
            level = 1;
            while (level == 1){
                attaqueDefenseur(&game);
                attaqueEnnemis(&game);
                actualiseJeu(&game, &plateau);
                
                if (affichage){
                    afficheJeuGraphique(&game);
                }
                else{
                    afficheJeuAscii(plateau);
                }


                level = condFinPartie(&game);
                
                MLV_wait_milliseconds(800);
                printf("\e[1;1H\e[2J");
            }
        }
        fclose(levels);
        if (level == 2){
            printf("Gagné !\n");
        }
        else{
            printf("Perdu :c\n");
        }
    }
    return 0;
}
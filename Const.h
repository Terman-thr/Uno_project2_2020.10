#ifndef MAIN_C_CONST_H
#define MAIN_C_CONST_H
#ifdef __WIN32
#define CLEAR() do { system("cls"); } while ((void)0, 0)
#else
#define CLEAR() do { system("clear"); } while ((void)0, 0)
#endif
#define MAX_CARD 20 // one player can have no more than 20 cards
#define MAX_WIDE_NUM 8

//structure of players
typedef struct _Player{
    int num;
    int score;
    int card[MAX_CARD]; //A player must have no more than MAX_CARD cards.
    struct _Player *next;
    struct _Player *prev;
}Player;

Player *Initialize(int n);
Player *Loop(int k,Player *player1);

#endif //MAIN_C_CONST_H

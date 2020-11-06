#include "Const.h"
#ifndef MAIN_C_TEST_H
#define MAIN_C_TEST_H
/*test whether the card can be played
 * Card_valid_Rec; the card player want to play*/
int TestCard(const int *Card_valid_Rec,const int *card);
int KExceed(int k,int n);//test whether k exceed range
int HandCardNum(Player *player);//calculate the number of hand cards of a player
int TestCardEmpty(const int *card,int d);//test number of card(specific)
int CalCardNum(const int *card);//calculate the number(>=0) of a pile of card
int FirstCardCmp(Player *player,int n);//choose the first player
int Winner(int n,Player *player,int *winner);//output the number(>=0) of winner

#endif //MAIN_C_TEST_H

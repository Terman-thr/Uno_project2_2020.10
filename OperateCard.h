#include <stdio.h>
#include "Const.h"
#ifndef MAIN_C_OPERATECARD_H
#define MAIN_C_OPERATECARD_H
void ShuffleCard(int CardNum,int *card, int a);// total card number ; the first card address
void ShuffleDiscardPile(int *discardpile, int *card, int d,FILE *fp);//Shuffle discard pile to stack when card is empty
void Dealer(int *card, Player *player);//Add cards to players: pointer of first card of stack; pointer of specific player

/*Play a card .i.e add a card to Card_record and DiscardPile;tidy player's card
 * the pointer of player; the sequence number of the card (0-MAX_CARD-1)*/
void PlayACard(Player *player,int num,int *Card_record,int *Card_valid_Rec,int *DiscardPile);
void bubble_sort(int *card, int len);//sort players' cards
#endif //MAIN_C_OPERATECARD_H

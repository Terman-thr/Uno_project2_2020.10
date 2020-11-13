#include "Const.h"
#include <stdio.h>
#ifndef MAIN_C_UI_H
#define MAIN_C_UI_H

void welcome(void);

void help(void);
/*turn the number(0-51) to corresponding card
 * the number(0-51) of the card*/
void Card2Str(int num);

/*turn the number(0-51) to corresponding card and write it into file
 * the number(0-51) of the card*/
void WCard2Str(int num,FILE *fp);

/*show what he played
 * the player1;the sequence number of the card ;k_1 */
void ShowPlayedCard(Player *player1,int CardNum,int k_1,FILE *fp);

void ShowHandCard(Player *player1,int k_1,FILE *fp);//show a player's hand card
void SingleCard2Image(int num);//display a single card

/*draw a ASCII image
 * number of cards; card pointer*/
void Card2Ima(int num,const int *card);

int GetValidInput(int Card_valid_Rec,int k,int k_1,int a_bool,Player *player1,int attack);//get valid input
#endif //MAIN_C_UI_H
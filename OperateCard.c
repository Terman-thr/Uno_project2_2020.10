#include "OperateCard.h"
#include "Const.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "UI.h"
#include "test.h"
// total card number ; the first card address
void ShuffleCard(int CardNum,int *card, int a){
    int tmp;
    srand(time(NULL));
    int ran;
    for (int i=0;i<CardNum;i++){
        ran=(rand()*a)%(CardNum);
        tmp=*(card+i);
        *(card+i)=*(card+(ran));
        *(card+(ran))=tmp;
    }
}

//Shuffle discard pile to stack when card is empty
void ShuffleDiscardPile(int *discardpile, int *card, int d,FILE *fp){
    int num = CalCardNum(discardpile);
    if (TestCardEmpty(card,d)){
        printf("\n\n We need to shuffle the discarded poker pile ...");
        fprintf(fp,"\n\n We need to shuffle the discarded poker pile ...");
        ShuffleCard(num, discardpile,rand());
        printf("\nShuffling cards ...\n");
        fprintf(fp,"\nShuffling cards ...\n");
        for (int i=0;i<num;i++)/* reuse the card */{
            *(card+(52*d-num)+i)=*(discardpile + i);
        }
        for (int i=0;i<num;i++)/* clean the discarded pile */{
            *(discardpile + i)=-1;
        }
        for (int j = 0; j < num; j++) {
            if (j%MAX_WIDE_NUM==0)
                fprintf(fp,"\n");
            WCard2Str(j,fp);
        }
        printf("\n");
    }
}

//Add cards to players: pointer of first card of stack; pointer of specific player
void Dealer(int *card, Player *player){
    int i=0,j=0;
    while (*(card+i)==-1) /* -1 represent for no card, draw card start from 0*/
    {   i++;}
    while ((*player).card[j]!=-1)
    {   j++;}
    (*player).card[j]=*(card+i);
    *(card+i)=-1;
}

/*Play a card .i.e add a card to Card_record and DiscardPile;tidy player's card
 * the pointer of player; the sequence number of the card (0-MAX_CARD-1)*/
void PlayACard(Player *player,int num,int *Card_record,int *Card_valid_Rec,int *DiscardPile){
    int k=0;
    if (*Card_valid_Rec==-1)/* the first round */{
        while(*(DiscardPile+k)!=-1)
        {   k++;}
        *(DiscardPile+k)=player->card[0];
        player->card[0]=-1;
    }
    else{
        if (num>-1) /* player doesn't choose to draw a card */{
            *Card_record = player->card[num];
            *Card_valid_Rec = player->card[num];
        }
        else{
            *Card_record=-1;}

        for (int i=num;i<MAX_CARD-1;i++){
            player->card[i]=player->card[i+1];
            player->card[MAX_CARD-1]=-1;
        }/*tidy the card*/
        while(*(DiscardPile+k)!=-1)
        {   k++;}
        *(DiscardPile+k)=*Card_valid_Rec; /* add card to discard pile */
    }
}

//sort players' cards
void bubble_sort(int *card, int len) {
    int i, j, temp;
    for (i = 0; i < len - 1; i++)
        for (j = 0; j < len - 1 - i; j++)
            if (card[j] < card[j + 1]) {
                temp = card[j];
                card[j] = card[j + 1];
                card[j + 1] = temp;
            }
}


//
// Created by 16006 on 2020/10/28.
//

#include "card_operator.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
/*int *set_card(void){
    int *card = calloc(52,sizeof(int));
    for (int i=0;i<=51;i++)
        card[i]=i;
    return card;
}*/

// desks ;total card number ; the first card address
void ShuffleCard(int CardNum,int *card){
    int tmp;
    srand((unsigned)time(NULL));
    for (int i=0;i<CardNum;i++){
        int ran=rand()%(CardNum);
        tmp=*(card+i);
        *(card+i)=*(card+(ran));
        *(card+(ran))=tmp;
    }
}

void *Card2Str(char *str,int num) {
    char *rank[4]={"J","Q","K","A"};
    char *suit[4]={"Spades","Hearts","Diamonds","Clubs"};
    if ((num%13+1)>10)
        sprintf(str,"%s %s",suit[num/13],rank[num%13-9]);
    else
        sprintf(str,"%s %d",suit[num/13],num%13+2);
}
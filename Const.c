#include "Const.h"
#include <stdio.h>
#include <stdlib.h>
Player *Loop(int k,Player *player1){
    Player  *result=player1;
    for (int i=0;i<k;i++){
        result=result->next;
    }
    return result;
}

//create n players, clean their hands and initialize their scores
Player *Initialize(int n) {
    Player *player,* head,* prev;
    player=(Player *)malloc(sizeof(Player));
    if(player==NULL){
        fprintf(stderr,"Failed to assign memory!\n");
        exit(-1);
    }
    head=player;
    prev=player;
    for (int i=0;i<n;i++){
        player->num=i;
        player->score=0;
        for (int j=0;j<MAX_CARD;j++) player->card[j]=-1;
        player->next=(Player *)malloc(sizeof(Player));
        player=player->next;
        player->prev=prev;
        prev=player;
    }
    player->next=head;
    head=player;
    player=player->next;
    player->prev=head;
    return player;
}

void DeletePlayer(Player *player1,int n){
    for (int i=0;i<n;i++){
        player1=player1->next;
        free(player1->prev);
    }
}
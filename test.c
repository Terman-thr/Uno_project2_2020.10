#include "test.h"
#include "Const.h"


/*test whether the card can be played
 * Card_valid_Rec; the card player want to play*/
int TestCard(const int *Card_valid_Rec,const int *card){
    if (*card<0)
        return 0;
    if (*Card_valid_Rec/13==*card/13 || *Card_valid_Rec%13==*card%13)
        return 1;
    else
        return 0;
}

//test whether k exceed range
int KExceed(int k,int n){
    if (k>=n){
        k=k-n;}
    else if (k<=-1){
        k=k+n;}
    return k;
}

//calculate the number of hand cards of a player
int HandCardNum(Player *player){
    int i=0;
    while (player->card[i]!=-1){
        i++;}
    return i;
}

//test number of card(specific)
int TestCardEmpty(const int *card,int d){
    if (card[52*d-1]==-1)
        return 1;//is empty
    else
        return 0;
}

//calculate the number(>=0) of a pile of card
int CalCardNum(const int *card){
    int i=0;
    while (*(card+i)!=-1){
        i++;}
    return i;
}


//choose the first player
int FirstCardCmp(Player *player,int n){
    int num=0;/*the number of player with max card*/
    int tmp=player->card[0];
    for (int i=1;i<n;i++){
        if (tmp<Loop(i,player)->card[0]){
            tmp=Loop(i,player)->card[0];
            num=i;
        }
    }
    return num;//if several player have same card, choose the previous player
}

//output the number(>=0) of winner
int Winner(int n,Player *player,int *winner){
    int winner_num=0;
    int max=player->score;
    for (int i=0;i<n;i++)/* get the max score */{
        max=(max<(Loop(i,player))->score)?((Loop(i,player))->score):(max);
    }
    for (int i=0;i<n;i++){
        if (max==(Loop(i,player))->score){
            *(winner+winner_num)=i;
            winner_num++;
        }
    }
    return winner_num;
}
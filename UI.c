#include <stdio.h>
#include "UI.h"
#include "Const.h"
void welcome(void){
    printf("########################\n"
           "#                      #\n"
           "# Welcome to One Card! #\n"
           "#                      #\n"
           "########################\n"
           "==============================================================\n\n\n\n\n");
}
void help(void){
    printf("-h|--help print this help message\n");
    printf("--log filename write the logs in filename (default: onecard.log)\n");
    printf("-n n|--player-number=n n players, n must be larger than 2 (default: 4)\n");
    printf("-c c|--initial-cards=c deal c cards per player, c must be at least 2 (default: 5)\n");
    printf("-d d|--decks=d use d decks 52 cards each, d must be at least 2 (default: 2)\n");
    printf("-r r|--rounds=r play r rounds, r must be at least 1 (default: 1)\n");
    printf("-a|--auto run in demo mode\n");
}

/*turn the number(0-51) to corresponding card
 * the number(0-51) of the card*/
void Card2Str(int num) {
    char *rank[4]={"J","Q","K","A"};
    char *suit[4]={"  Spades","  Hearts","Diamonds","   Clubs"};
    if ((num % 13 + 1) >= 10)
        printf("%s %s   ", suit[num / 13], rank[num % 13 - 9]);
    else
        printf("%s %d   ", suit[num / 13], num % 13 + 2);
}

/*turn the number(0-51) to corresponding card and write it into file
 * the number(0-51) of the card*/
void WCard2Str(int num,FILE *fp) {
    char *rank[4]={"J","Q","K","A"};
    char *suit[4]={"  Spades","  Hearts","Diamonds","   Clubs"};
    if ((num % 13 + 1) >= 10)
        fprintf(fp,"%s %s   ", suit[num / 13], rank[num % 13 - 9]);
    else
        fprintf(fp,"%s %d   ", suit[num / 13], num % 13 + 2);
}

/*show what he played
 * the player1;the sequence number of the card ;k_1 */
void ShowPlayedCard(Player *player1,int CardNum,int k_1,FILE *fp) {
    printf("\n\nPlayer %d plays : ", k_1 + 1);
    fprintf(fp,"\n\nPlayer %d plays : ", k_1 + 1);
    Card2Str((Loop(k_1,player1))->card[CardNum]);
    WCard2Str((Loop(k_1,player1))->card[CardNum],fp);
    printf("\nPlayer %d's card(s) : ", k_1 + 1);
    fprintf(fp,"\nPlayer %d's card(s) : ", k_1 + 1);
}

/* show a player's hand card */
void ShowHandCard(Player *player1,int k_1,FILE *fp){
    int t=0;
    while ((Loop(k_1,player1))->card[t]!=-1){
        Card2Str((Loop(k_1,player1))->card[t]);
        WCard2Str((Loop(k_1,player1))->card[t],fp);
        printf("  ");
        fprintf(fp,"  ");
        t++;
    }
}


/*draw a ASCII image
 * number of cards; card pointer*/
void Card2Ima(int num,const int *card){
    char *rank[4]={"J","Q","K","A"};
    char *suit[4]={"S","H","D","C"};
    for (int i=0;i<num;i++)/*the upper raw*/{
        printf("-------    ");}
    printf("\n");
    for (int i=0;i<num;i++)/*the second raw*/{
        if (card[i]%13==8){
            printf("|10");
        }
        else if (card[i]%13>8){
            printf("|%s ",rank[card[i]%13-9]);
        }
        else{
            printf("|%d ",card[i]%13+2);
        }
        printf("   |    ");
    }
    printf("\n");
    for (int i=0;i<num;i++)/*the third raw*/{
        printf("|  %s  |",suit[card[i]/13]);
        printf("    ");
    }
    printf("\n");
    for (int i=0;i<num;i++)/*the fourth raw*/{
        printf("|   ");
        if (card[i]%13==8){
            printf("10|");
        }
        else if (card[i]%13>8){
            printf(" %s|",rank[card[i]%13-9]);
        }
        else{
            printf(" %d|",card[i]%13+2);
        }
        printf("    ");
    }
    printf("\n");
    for (int i=0;i<num;i++)/*the lower raw*/{
        printf("-------    ");}
    printf("\n");

    for (int i=0;i<num;i++){
        printf("  %2d       ",i);
    }
    printf("\n");
}

//display a single card
void SingleCard2Image(int num){
    char *rank[4]={"J","Q","K","A"};
    char *suit[4]={"S","H","D","C"};
    printf("-------\n");
    if (num%13==8){
        printf("|10");
    }
    else if (num%13>8){
        printf("|%s ",rank[num%13-9]);
    }
    else{
        printf("|%d ",num%13+2);
    }
    printf("   |\n");
    printf("|  %s  |\n",suit[num/13]);
    printf("|   ");
    if (num%13==8){
        printf("10|\n");
    }
    else if (num%13>8){
        printf(" %s|\n",rank[num%13-9]);
    }
    else{
        printf(" %d|\n",num%13+2);
    }
    printf("-------\n");
}


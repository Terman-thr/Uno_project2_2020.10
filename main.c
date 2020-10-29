#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "UI.h"
#include "card_operator.h"
#include "poker_total_card.h"
#include <getopt.h>
int str2num(const char *num){
    int ans=0,i=0;
    while ((*(num+i))!='\0'){
        ans=10*ans+*(num+i)-48;
        i++;
    }
    return ans;
}


int main(int argc,char*argv[]) {
    welcome();

    int opt;
    char *filename="onecard.log";
    int n=4,c=5,d=2,r=1;/*default numbers*/
    int a_bool =0;/*0 for demo, 1 for play*/   //temporarily set a_bool be 0;
    int i,option_index = 0;
    char string[10]="n:r:d:c:h";
    static struct option long_options[] =
            {
                    {"help", no_argument, NULL, 'h'},
                    {"log", required_argument, NULL, 'l'},
                    {"player-number",  required_argument,NULL, 'n'},
                    {"initial-cards",  required_argument,NULL, 'c'},
                    {"decks",required_argument,NULL, 'd'},
                    {"rounds",required_argument,NULL, 'r'},
                    {NULL, 0,	                  NULL, 0},
            };

    //get the command
    while((opt = getopt_long_only(argc, argv, string, long_options,&option_index)) != -1)
    {
        switch (opt) {
            case 'h':
                help();
                break;
            case 'r':
                r=str2num(optarg);
                break;
            case 'n':
                n=str2num(optarg);
                break;
            case 'd':
                d=str2num(optarg);
                break;
            case 'c':
                c=str2num(optarg);
                break;
            case 'l':
                filename=optarg;
                break;
            default:
                printf("Invalid input!");
        }
    }
    printf(" r=%d\n d=%d\n n=%d\n c=%d\n filename=%s\n",r,d,n,c,filename);

    //-------------------------------------the card part-------------------------------------------------------
    //define the structure of players
    typedef struct _Player{
        int num;
        int card[100];
    }Player;

    //create n players and clean their hands
    Player player[n];
    for (int i=0;i<n;i++){
        for (int j=1;j<=100;j++)
            player[i].card[j]=0;
    }
    //init the stack, which are d desks of pokers
    int *card=(int *)calloc(d*52,sizeof(int));
    for (int i=1;i<=d;i++){
        for (int j=0;j<=51;j++)
            *(card+(i-1)*52+j)=j;
    }
    ShuffleCard(52*d,card);

    return 0;
}
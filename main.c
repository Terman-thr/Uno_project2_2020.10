#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#define MAX_CARD 20 // one player can have no more than 20 cards
#define MAX_WIDE_NUM 8
//structure of players
typedef struct _Player{
    int score;
    int card[MAX_CARD]; //A player must have no more than MAX_CARD cards.
}Player;

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
int str2num(const char *num){
    int ans=0,i=0;
    while ((*(num+i))!='\0'){
        ans=10*ans+*(num+i)-48;
        i++;
    }
    return ans;
}

// total card number ; the first card address
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

/*turn the number(0-51) to corresponding card
 * the string store the card; the number(0-51) of the card*/
void Card2Str(char *str,int num) {
    char *rank[4]={"J","Q","K","A"};
    char *suit[4]={"  Spades","  Hearts","Diamonds","   Clubs"};
    if ((num%13+1)>10)
        sprintf(str,"%s %s",suit[num/13],rank[num%13-9]);
    else
        sprintf(str,"%s %d",suit[num/13],num%13+2);
    if ((num % 13 + 1) > 10)
        printf("%s %s   ", suit[num / 13], rank[num % 13 - 9]);
    else
        printf("%s %d   ", suit[num / 13], num % 13 + 2);

}

//Add cards to players: pointer of first card of stack; pointer of specific player
void Dealer(const int *card, Player *player){
    int i=0,j=0;
    while (*(card+i)!=-1) /* -1 represent for no card, draw card start from 0*/
    {
        while ((*player).card[j]==-1){
            (*player).card[j]=*(card+i);
            j++;}
        i++;}
}

int FirstCardCmp(Player *player,int n){
    int num;/*the number of player with max card*/
    int tmp=(*player).card[0];
    for (int i=1;i<n;i++){
        if (tmp<(*(player+i)).card[0]){
            tmp=(*(player+i)).card[0];
            num=i+1;
        }
    }
    return num;//if several player have same card, choose the previous player
}

//Add card from stack or initial stack to players
int main(int argc,char*argv[]) {
    welcome();
    int opt;
    char *filename="onecard.log";
    int n=4,c=5,d=2,r=1;/*default numbers*/
    int a_bool =0;/*1 for demo, 0 for players*/   //temporarily set a_bool be 0;
    int option_index = 0;
    char string[11]="n:r:d:c:ha";
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

    //deal with the command
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
            case 'a':
                a_bool=1;
                break;
            default:
                printf("Invalid input!");
        }
    }
    printf("Showing the number you set.\n r=%d\n d=%d\n n=%d\n c=%d\n filename=%s\n",r,d,n,c,filename);

    //-------------------------------------the card part-------------------------------------------------------

    //create n players, clean their hands and initialize their scores
    Player player[n];
    Player *player1=&player[0];
    for (int i=0;i<n;i++){
        player[i].score=0;
        for (int j=1;j<=MAX_CARD;j++)
            player[i].card[j]=-1;// -1>>no card
    }

    //init the stack, which are d desks of pokers
    int *card=(int *)calloc(d*52,sizeof(int));
    for (int i=1;i<=d;i++){
        for (int j=0;j<=51;j++)
            *(card+(i-1)*52+j)=j;
    }/* the card is now the pointer of the first 0 0-51->0-51*/

    //Shuffle the cards
    ShuffleCard(52 * d, card);
    printf("\n\nShuffling cards,please wait...\n\n\n");

    //show the Shuffled cards in  **demo**  mode
    if (a_bool) {
        printf("Initial cards stack:");
        char sShowCard[52 * d][11];
        for (int j = 0; j < 52 * d; j++) {
            if (j%MAX_WIDE_NUM==0)
                printf("\n\n");
            Card2Str(sShowCard[j], *(card + j));
        }
    }

    //Draw the first card and compare
    for (int i=0;i<n;i++)
        Dealer(card,player1+i);
    int first=FirstCardCmp(player1,n);
    printf("%d",first);
    return 0;
}
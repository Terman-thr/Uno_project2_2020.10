#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

#ifdef __WIN32
#define CLEAR() do { system("cls"); } while ((void)0, 0)
#else
#define CLEAR() do { system("clear"); } while ((void)0, 0)
#endif

#define MAX_CARD 20 // one player can have no more than 20 cards
#define MAX_WIDE_NUM 8
//structure of players
typedef struct _Player{
    int score;
    int card[MAX_CARD]; //A player must have no more than MAX_CARD cards.
}Player;

//quick
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

//calculate the number(>=0) of a pile of card
int CalCardNum(const int *card){
    int i=0;
    while (*(card+i)!=-1){
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

//choose the first player
int FirstCardCmp(Player *player,int n){
    int num=0;/*the number of player with max card*/
    int tmp=player->card[0];
    for (int i=1;i<n;i++){
        if (tmp<(player+i)->card[0]){
            tmp=(player+i)->card[0];
            num=i;
        }
    }
    return num;//if several player have same card, choose the previous player
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

//output the number(>=0) of winner
int Winner(int n,Player *player,int *winner){
    int winner_num=0;
    int max=player->score;
    for (int i=0;i<n;i++)/* get the max score */{
        max=(max<(player+i)->score)?((player+i)->score):(max);
    }
    for (int i=0;i<n;i++){
        if (max==(player+i)->score){
            *(winner+winner_num)=i;
            winner_num++;
        }
    }
    return winner_num;
}

/*show what he played
 * the player1;the sequence number of the card ;k_1 */
void ShowPlayedCard(Player *player1,int CardNum,int k_1,FILE *fp) {

    printf("\n\nPlayer %d plays : ", k_1 + 1);
    fprintf(fp,"\n\nPlayer %d plays : ", k_1 + 1);
    Card2Str((player1 + k_1)->card[CardNum]);
    WCard2Str((player1 + k_1)->card[CardNum],fp);
    printf("\nPlayer %d's card(s) : ", k_1 + 1);
    fprintf(fp,"\nPlayer %d's card(s) : ", k_1 + 1);
}

/* show a player's hand card */
void ShowHandCard(Player *player1,int k_1,FILE *fp){
    int t=0;
    while ((player1 + k_1)->card[t]!=-1){
        Card2Str((player1 + k_1)->card[t]);
        WCard2Str((player1 + k_1)->card[t],fp);
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
                return 0;
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
    //define the file
    FILE *fp=fopen(filename,"w");
    printf("---- Initial setup ----\n Number of rounds: %d\n Number of decks: %d\n"
           " Number of players: %d\n Number of initial cards: %d\n File name of log :%s\n",r,d,n,c,filename);
    fprintf(fp,"---- Initial setup ----\n Number of rounds: %d\n Number of decks: %d\n"
           " Number of players: %d\n Number of initial cards: %d\n File name of log :%s\n",r,d,n,c,filename);

    //-------------------------------------the card part----------------------------------------------------------------


    //create n players, clean their hands and initialize their scores
    Player player[n];
    Player *player1=&player[0];
    for (int i=0;i<n;i++){
        player[i].score=0;
        for (int j=0;j<MAX_CARD;j++)
            player[i].card[j]=-1;// -1>>no card
    }

    //init the stack, which are d desks of pokers
    int *card=(int *)calloc(d*52,sizeof(int));
    for (int i=1;i<=d;i++){
        for (int j=0;j<=51;j++)
            *(card+(i-1)*52+j)=j;
    }/* the card is now the pointer of the first 0 0-51->0-51*/

    //Shuffle the cards
    ShuffleCard(52 * d, card,rand());
    printf("\n\nShuffling cards,please wait...\n\n\n");
    fprintf(fp,"\n\nShuffling cards,please wait...\n\n\n");

    //show the Shuffled cards in  **demo**  mode
    if (a_bool) {
        printf("Shuffle result:\n");
        for (int j = 0; j < 52 * d; j++) {
            if (j%MAX_WIDE_NUM==0)
                printf("\n");
            Card2Str(*(card + j));
        }
    }
    //show the Shuffled cards in file
    fprintf(fp,"Shuffle result:\n");
    for (int j = 0; j < 52 * d; j++) {
        if (j%MAX_WIDE_NUM==0)
            fprintf(fp,"\n");
        WCard2Str(*(card + j),fp);
    }

    //Initialize the discard pile
    int *DiscardPile=(int *)calloc(d*52,sizeof(int));
    for (int i=0;i<d*52;i++){
        *(DiscardPile+i)=-1;
    }/* the card is now the pointer of the first 0  all -1*/

    //Draw the first card and compare
    for (int i=0;i<n;i++)
        Dealer(card,player1+i);
    int first=FirstCardCmp(player1,n);/* first : the first player showing cards*/
    printf("\nDetermining the playing order...\n\n");
    fprintf(fp,"\nDetermining the playing order...\n\n");

    /* Show first cards in all mode*/
    for (int i=0;i<n;i++){
        printf("Player %d : ",i+1);
        fprintf(fp,"Player %d : ",i+1);
        Card2Str(player[i].card[0]);
        WCard2Str(player[i].card[0],fp);
        printf("\n");
        fprintf(fp,"\n");
    }
    printf("\nGame will start with player %d.\n\n\n",first+1);
    fprintf(fp,"\nGame will start with player %d.\n\n\n",first+1);

    //Create the play record and initialize it
    int Card_Record=-1,Card_valid_Rec=-1;

    //Discard the first card
    for (int i=0;i<n;i++){
        PlayACard((player1+i),0,&Card_Record,&Card_valid_Rec,DiscardPile);}

    //give each player c cards
    printf("Dealing cards...\n");
    fprintf(fp,"Dealing cards...\n");
    for (int i=0;i<n;i++){
        for (int j=0;j<c;j++){
            Dealer(card,(player1+i));}
    }

    //sort players' cards
    for (int i=0;i<n;i++)
        bubble_sort((player1+i)->card,HandCardNum(player1+i));

    /* show what players have in **demo** mode */
    if (a_bool){
        for (int i=0;i<n;i++){
            printf("\n Player %d:",i+1);
            fprintf(fp,"\n Player %d:",i+1);
            for (int j=0;j<c;j++){
                Card2Str(player[i].card[j]);
                WCard2Str(player[i].card[j],fp);
            }
        }
    }

    //-------------------------------------the play part (demo mode)----------------------------------------------------

    //deal the first card of stack ;initialize the suit and rank
    int k=0,t=0;
    while (card[k]==-1){
        k++;}
    Card_Record=card[k];
    Card_valid_Rec=card[k];
    DiscardPile[n]=card[k];
    card[k]=-1;
    printf("\n\n==========================Game Start============================\n\nFirst card : ");
    fprintf(fp,"\n\n==========================Game Start============================\n\nFirst card : ");
    Card2Str(Card_valid_Rec);
    WCard2Str(Card_valid_Rec,fp);

    printf("\nPress Enter to continue\n");
    getchar();

    //the play cards part
    k=first; //the first one to play the card.
    int Hand_poker_num;
    int SerialNum,k_1=k;
    /* SerialNum: the number of cards in player's hand
    * k_1: test the Q(jump card)*/
    srand((unsigned)time(NULL));
    int r_1=1;

    // ==================================the main part of playing cards==============================================
    //===============================demo mode==========================================
    if (a_bool){
        while (r > 0) {
            printf("\n\n==========================Round %d============================\n\n", r_1);
            fprintf(fp,"\n\n==========================Round %d============================\n\n", r_1);
            printf("First card : ");
            fprintf(fp,"First card : ");
            Card2Str(Card_valid_Rec);
            printf("\n\n Game starts with Player %d\n", k_1 + 1);
            fprintf(fp,"\n\n Game starts with Player %d\n", k_1 + 1);
            printf("Dealing cards...\n");
            fprintf(fp,"Dealing cards...\n");

            int attack = 0, direction = 1;// 1 for clockwise 0 for counter-clockwise
            while ((player1 + k_1)->card[0] != -1) /* one player used all his/her cards */
            {
                //sort players' cards
                for (int i=0;i<n;i++)
                    bubble_sort((player1+i)->card,HandCardNum(player1+i));

                k_1 = k;//adjust the k
                /* ------------------------------if there is no attack--------------------------- */
                if (attack == 0) {
                    for (SerialNum = 0; SerialNum < MAX_CARD + 1; SerialNum++) {
                        //ensure the card serial number is valid
                        if (SerialNum == MAX_CARD)/* the player have to draw a card */{
                            ShuffleDiscardPile(DiscardPile, card, d,fp);
                            Dealer(card, player1 + k);
                            printf("\n\nPlayer %d draws: ", k_1 + 1); //demo mode
                            fprintf(fp,"\n\nPlayer %d draws: ", k_1 + 1);

                            while ((player1 + k_1)->card[t] != -1) { t++; }
                            Card2Str((player1 + k_1)->card[t - 1]);
                            WCard2Str((player1 + k_1)->card[t - 1],fp);
                            t = 0;

                            printf("\nPlayer %d's card(s): ", k_1 + 1);//show
                            fprintf(fp,"\nPlayer %d's card(s): ", k_1 + 1);
                            bubble_sort((player1+k)->card,HandCardNum(player1+k_1));
                            while ((player1 + k_1)->card[t] != -1) {
                                Card2Str((player1 + k_1)->card[t]);
                                WCard2Str((player1 + k_1)->card[t],fp);
                                printf("  ");
                                fprintf(fp,"  ");
                                t++;
                            }
                            t = 0;//reset the t;
                            break;
                        }
                        if (TestCard(&Card_valid_Rec, &(player1 + k)->card[SerialNum])) /* fit rank or suit */
                        {
                            printf("\n\nPlayer %d plays: ", k_1 + 1); //demo mode
                            fprintf(fp,"\n\nPlayer %d plays: ", k_1 + 1);
                            Card2Str((player1 + k)->card[SerialNum]);
                            WCard2Str((player1 + k)->card[SerialNum],fp);

                            switch ((player1 + k)->card[SerialNum] % 13) /* test whether they are special cards */{
                                case 9: //J
                                    k = direction ? (k + 1) : (k - 1);
                                    break;
                                case 10: //Q
                                    direction = !direction;
                                    break;
                                case 0: //2
                                    attack = 2;
                                    break;
                                case 1: //3
                                    attack = 3;
                                    break;
                            }
                            PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                            //show the cards
                            printf("\nPlayer %d's card(s): ", k_1 + 1);//demo mode
                            fprintf(fp,"\nPlayer %d's card(s): ", k_1 + 1);
                            while ((player1 + k_1)->card[t] != -1)/* show */{
                                Card2Str((player1 + k_1)->card[t]);
                                WCard2Str((player1 + k_1)->card[t],fp);
                                printf("  ");
                                fprintf(fp,"  ");
                                t++;
                            }
                            t = 0;//reset the t
                            break;
                        }
                    }
                    k = direction ? (k + 1) : (k - 1);

                    //test whether k exceed the range
                    k = KExceed(k, n);
                }
                    /* ------------------------there is an attack --------------------------*/
                else {
                    Hand_poker_num = HandCardNum(player1 + k_1);
                    for (SerialNum = 0; SerialNum < MAX_CARD + 1; SerialNum++) {
                        //ensure the card serial number is valid
                        if (SerialNum == MAX_CARD)/* player have no card to play i.e. draw many cards */{
                            for (int i = 0; i < attack; i++)/* draw card*/{
                                ShuffleDiscardPile(DiscardPile, card, d,fp);
                                Dealer(card, player1 + k_1);
                            }
                            printf("\n\nPlayer %d draws :", k_1 + 1);
                            fprintf(fp,"\n\nPlayer %d draws :", k_1 + 1);
                            for (t = 0; t < attack; t++) {
                                Card2Str((player1 + k_1)->card[Hand_poker_num + t]);
                                WCard2Str((player1 + k_1)->card[Hand_poker_num + t],fp);
                                printf("   ");
                                fprintf(fp,"   ");
                            }
                            t = 0;

                            attack = 0;
                            printf("\nPlayer %d's card(s): ", k_1 + 1);//show
                            fprintf(fp,"\nPlayer %d's card(s): ", k_1 + 1);
                            bubble_sort((player1+k)->card,HandCardNum(player1+k_1));
                            while ((player1 + k_1)->card[t] != -1) {
                                Card2Str((player1 + k_1)->card[t]);
                                WCard2Str((player1 + k_1)->card[t],fp);
                                printf("  ");
                                fprintf(fp,"  ");
                                t++;
                            }
                            t = 0;
                            break;
                        }

                        /* if he has special cards */
                        if (TestCard(&Card_valid_Rec, &(player1 + k)->card[SerialNum])/* not fit rank or suit */
                            && ((player1 + k)->card[SerialNum] % 13 == 5
                                || (player1 + k)->card[SerialNum] % 13 == 0
                                || (player1 + k)->card[SerialNum] % 13 == 1
                                || (player1 + k)->card[SerialNum] % 13 == 9
                                || (player1 + k)->card[SerialNum] % 13 == 10)) {
                            switch ((player1 + k)->card[SerialNum] % 13) {
                                case 5: //7
                                    attack = 0;
                                    ShowPlayedCard(player1, SerialNum, k_1,fp);
                                    PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                    ShowHandCard(player1, k_1,fp);
                                    break;
                                case 0: //2
                                    attack += 2;
                                    ShowPlayedCard(player1, SerialNum, k_1,fp);
                                    PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                    ShowHandCard(player1, k_1,fp);
                                    break;
                                case 1: //3
                                    attack += 3;
                                    ShowPlayedCard(player1, SerialNum, k_1,fp);
                                    PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                    ShowHandCard(player1, k_1,fp);
                                    break;
                                case 9: //J
                                    k = direction ? (k + 1) : (k - 1);
                                    ShowPlayedCard(player1, SerialNum, k_1,fp);
                                    PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                    ShowHandCard(player1, k_1,fp);
                                    break;
                                case 10: //Q
                                    direction = !direction;
                                    ShowPlayedCard(player1, SerialNum, k_1,fp);
                                    PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                    ShowHandCard(player1, k_1,fp);
                                    break;
                            }
                            break;
                        }
                    }
                    k = direction ? (k + 1) : (k - 1);
                    //test whether k exceed the range
                    k = KExceed(k, n);
                }
            }
            //---------------------------------One round finished----------------------------
            printf("\n\nPlayer %d wins this round!!!\n\n", k_1 + 1);
            fprintf(fp,"\n\nPlayer %d wins this round!!!\n\n", k_1 + 1);

            for (int i = 1; i <= d; i++) {
                for (int j = 0; j <= 51; j++)
                    *(card + (i - 1) * 52 + j) = j;
            }/* the card is now the pointer of the first 0 0-51->0-51*/
            ShuffleCard(52 * d, card, rand());

            for (int i = 0; i < d * 52; i++)/* clean discard pile */{
                DiscardPile[i] = -1;
            }
            printf("\nPreparing for another round...\n");
            printf("\n\nShuffling cards,please wait...\n\n\n");
            fprintf(fp,"\nPreparing for another round...\n");
            fprintf(fp,"\n\nShuffling cards,please wait...\n\n\n");

            printf("\n---- Stats ----\nRound %d Result :\n\n", r_1);
            fprintf(fp,"\n---- Stats ----\nRound %d Result :\n\n", r_1);
            for (int i = 0; i < n; i++)/* calculate players score */{
                (player1 + i)->score = (player1 + i)->score - HandCardNum(player1 + i);
                printf("Player %d :%d ,total: %d\n ", i + 1,- HandCardNum(player1 + i), (player1 + i)->score);
                fprintf(fp,"Player %d :%d ,total: %d\n ", i + 1,- HandCardNum(player1 + i), (player1 + i)->score);
            }
            printf("\n\nRound %d ends \n\n",r_1);
            fprintf(fp,"\n\nRound %d ends \n\n",r_1);

            //clean players' hand poker and re-deal cards
            for (int i = 0; i < n; i++)/* clean hands */{
                for (int j = 0; j < MAX_CARD; j++)
                    player[i].card[j] = -1;// -1>>no card
            }
            Card_Record = card[0];
            Card_valid_Rec = card[0];
            DiscardPile[0] = card[0];
            card[0] = -1;

            if (r-1>=0) {
                //give each player c cards
                printf("Dealing cards...\n");
                fprintf(fp, "Dealing cards...\n");
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < c; j++) {
                        Dealer(card, (player1 + i));
                    }
                }
            }
            k = k_1;
            r--;
            r_1++;
        }
    }
        //===============================Player mode==========================================
    else{
        while (r > 0) {
            printf("\n\n==========================Round %d============================\n\n", r_1);
            fprintf(fp,"\n\n==========================Round %d============================\n\n", r_1);
            printf("\n\n Game starts with Player %d\n", k_1+1);
            printf("Dealing cards...\n");
            fprintf(fp,"\n\n Game starts with Player %d\n", k_1+1);
            fprintf(fp,"Dealing cards...\n");
            int attack = 0, direction = 1;// 1 for clockwise 0 for counter-clockwise
            while ((player1 + k_1)->card[0] != -1) /* one player used all his/her cards */
            {

                //sort players' cards
                for (int i=0;i<n;i++)
                    bubble_sort((player1+i)->card,HandCardNum(player1+i));

                int valid=0;/* whether players' input are valid */
                k_1 = k;//adjust the k
                printf("\nThe previous Card\n");
                SingleCard2Image(Card_valid_Rec);
                /* ------------------------------if there is no attack--------------------------- */
                if (attack == 0) {

                    //show player's card
                    printf("\n\nYour (Player %d's) card(s): \n", k_1 + 1);
                    fprintf(fp,"\n\nPlayer %d's card(s): \n", k_1 + 1);

                    Card2Ima(HandCardNum(player1+k_1),(player1+k_1)->card);
                    while ((player1 + k_1)->card[t] != -1) {
                        WCard2Str((player1 + k_1)->card[t],fp);
                        fprintf(fp,"  ");
                        t++;
                    }
                    t = 0;//reset the t;

                    //Get valid input
                    do{
                        printf("\nPlease choose the card you play and enter the serial number under the card\n ");
                        printf("Enter -1 for draw a card.(No attack)\n");

                        //exclude invalid input
                        if (scanf("%d",&SerialNum)) {
                            if ((SerialNum >= -1 && SerialNum < HandCardNum(player1 + k_1)
                                 && TestCard(&Card_valid_Rec, &(player1 + k)->card[SerialNum]))
                                ||SerialNum == -1) {
                                valid=1;
                            }
                            else{
                                printf("\n Invalid input!!! Please enter again!\n");
                            }
                        }
                    }while (valid==0);

                    //ensure the card serial number is valid
                    if (SerialNum == -1)/* the player have to draw a card */{
                        ShuffleDiscardPile(DiscardPile, card, d,fp);
                        Dealer(card, player1 + k);
                        printf("\n\nYou (Player %d) draws: \n", k_1 + 1);
                        fprintf(fp,"\n\nPlayer %d draws: \n", k_1 + 1);
                        //show card player draws
                        SingleCard2Image((player1+k_1)->card[HandCardNum(player1+k_1)-1]);
                        WCard2Str((player1+k_1)->card[HandCardNum(player1+k_1)-1],fp);

                        printf("\nPress Enter to continue\n");
                        getchar();
                        getchar();
                    }
                    else /* fit rank or suit */
                    {
                        printf("\n\nYou (Player %d) plays: \n", k_1 + 1); //demo mode
                        fprintf(fp,"\n\nPlayer %d plays: \n", k_1 + 1);
                        SingleCard2Image((player1 + k)->card[SerialNum]);
                        WCard2Str((player1 + k)->card[SerialNum],fp);
                        switch ((player1 + k)->card[SerialNum] % 13) /* test whether they are special cards */{
                            case 9: //J
                                k = direction ? (k + 1) : (k - 1);
                                break;
                            case 10: //Q
                                direction = !direction;
                                break;
                            case 0: //2
                                attack = 2;
                                break;
                            case 1: //3
                                attack = 3;
                                break;
                        }
                        PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                        printf("\nPress Enter to continue\n");
                        getchar();
                        getchar();
                    }

                    k = direction ? (k + 1) : (k - 1);

                    //test whether k exceed the range
                    k = KExceed(k, n);
                }
                    /* ------------------------there is an attack --------------------------*/
                else {
                    Hand_poker_num = HandCardNum(player1 + k_1);

                    t=0;
                    printf("\nYour (Player %d's) card(s): \n", k_1 + 1);//show
                    fprintf(fp,"\nPlayer %d's card(s): \n", k_1 + 1);
                    Card2Ima(Hand_poker_num,(player1+k_1)->card);
                    while ((player1 + k_1)->card[t] != -1) {
                        WCard2Str((player1 + k_1)->card[t],fp);
                        fprintf(fp,"  ");
                        t++;
                    }

                    //Get valid input
                    do{
                        printf("\nPlease choose the card you play and enter the serial number under the card\n ");
                        printf("Enter -1 for draw a card.(An attack)\n");

                        //exclude invalid input
                        if (scanf("%d",&SerialNum)) {
                            if ((SerialNum >= -1 && SerialNum < Hand_poker_num
                                 && (TestCard(&Card_valid_Rec, &(player1 + k)->card[SerialNum])/* not fit rank or suit */
                                     && ((player1 + k)->card[SerialNum] % 13 == 5
                                         || (player1 + k)->card[SerialNum] % 13 == 0
                                         || (player1 + k)->card[SerialNum] % 13 == 1
                                         || (player1 + k)->card[SerialNum] % 13 == 9
                                         || (player1 + k)->card[SerialNum] % 13 == 10)))
                                || SerialNum == -1) {
                                valid=1;
                            }
                            else{
                                printf("\n Invalid input!!! Please enter choose a special card or draw cards!!!\n");
                            }
                        }
                    }while (valid==0);

                    //ensure the card serial number is valid
                    if (SerialNum ==-1)/* player have no card to play i.e. draw many cards */{
                        for (int i = 0; i < attack; i++)/* draw card*/{
                            ShuffleDiscardPile(DiscardPile, card, d,fp);
                            Dealer(card, player1 + k_1);
                        }
                        printf("\n\nYou (Player %d) draw :\n", k_1 + 1);
                        fprintf(fp,"\n\nPlayer %d draw :\n", k_1 + 1);
                        Card2Ima(attack,(player1+k_1)->card+Hand_poker_num);
                        for (int i=0;i<attack;i++) {
                            WCard2Str(*((player1 + k_1)->card + Hand_poker_num+i), fp);
                        }
                        printf("\nPress Enter to continue\n");
                        getchar();
                        getchar();
                        attack = 0;
                    }
                        /* if he has special cards */
                    else{
                        switch ((player1 + k_1)->card[SerialNum] % 13) {
                            case 5: //7
                                attack = 0;
                                printf("You played :\n");
                                fprintf(fp,"Player played :\n");
                                WCard2Str((player1 + k_1)->card[SerialNum],fp);
                                SingleCard2Image((player1 + k_1)->card[SerialNum]);
                                PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                break;
                            case 0: //2
                                attack += 2;
                                printf("You played :\n");
                                fprintf(fp,"Player played :\n");
                                WCard2Str((player1 + k_1)->card[SerialNum],fp);
                                SingleCard2Image((player1 + k_1)->card[SerialNum]);
                                PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                break;
                            case 1: //3
                                attack += 3;
                                printf("You played :\n");
                                fprintf(fp,"Player played :\n");
                                WCard2Str((player1 + k_1)->card[SerialNum],fp);
                                SingleCard2Image((player1 + k_1)->card[SerialNum]);
                                PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                break;
                            case 9: //J
                                k = direction ? (k + 1) : (k - 1);
                                printf("You played :\n");
                                fprintf(fp,"Player played :\n");
                                WCard2Str((player1 + k_1)->card[SerialNum],fp);
                                SingleCard2Image((player1 + k_1)->card[SerialNum]);
                                PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                break;
                            case 10: //Q
                                direction = !direction;
                                printf("You played :\n");
                                fprintf(fp,"Player played :\n");
                                WCard2Str((player1 + k_1)->card[SerialNum],fp);
                                SingleCard2Image((player1 + k_1)->card[SerialNum]);
                                PlayACard(player1 + k_1, SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                break;
                        }
                        printf("\nPress Enter to continue\n");
                        getchar();
                        getchar();
                    }
                    k = direction ? (k + 1) : (k - 1);
                    //test whether k exceed the range
                    k = KExceed(k, n);
                }
                CLEAR();
                printf("\nPress Enter to continue\n");
                printf("Next Player :Player %d",k+1);
                getchar();
                CLEAR();
            }
            //---------------------------------One round finished----------------------------
            printf("\n\nPlayer %d wins this round!!!\n\n", k_1 + 1);
            fprintf(fp,"\n\nPlayer %d wins %d round.\n\n", k_1 + 1,r_1);

            for (int i = 1; i <= d; i++) {
                for (int j = 0; j <= 51; j++)
                    *(card + (i - 1) * 52 + j) = j;
            }/* the card is now the pointer of the first 0 0-51->0-51*/
            ShuffleCard(52 * d, card, rand());

            for (int i = 0; i < d * 52; i++)/* clean discard pile */{
                DiscardPile[i] = -1;
            }
            printf("Preparing for another round...\n");
            printf("\n\nShuffling cards,please wait...\n\n\n");


            printf("\n---- Stats ----\nRound %d result:\n\n", r_1);
            fprintf(fp,"\n---- Stats ----\nRound %d result:\n\n", r_1);
            for (int i = 0; i < n; i++)/* calculate players score */{
                (player1 + i)->score = (player1 + i)->score - HandCardNum(player1 + i);
                printf("Player %d :%d ,total: %d\n ", i + 1,- HandCardNum(player1 + i), (player1 + i)->score);
                fprintf(fp,"Player %d :%d ,total: %d\n ", i + 1,- HandCardNum(player1 + i), (player1 + i)->score);
            }
            printf("\n\nRound %d ends \n\n",r_1);
            fprintf(fp,"\n\nRound %d ends \n\n",r_1);

            //clean players' hand poker and re-deal cards
            for (int i = 0; i < n; i++)/* clean hands */{
                for (int j = 0; j < MAX_CARD; j++)
                    player[i].card[j] = -1;// -1>>no card
            }
            Card_Record = card[0];
            Card_valid_Rec = card[0];
            DiscardPile[0] = card[0];
            card[0] = -1;

            if (r-1<=0) {
                //give each player c cards
                printf("Dealing cards...\n");
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < c; j++) {
                        Dealer(card, (player1 + i));
                    }
                }
            }
            k = k_1;
            r--;
            r_1++;
        }
    }
    int Winner_num;
    int *winner=(int *)calloc(n,sizeof(int));
    for (int i=0;i<n;i++)/* initialize the winner array */{
        *(winner+i)=-1;
    }

    Winner_num=Winner(n,player1,winner);
    printf("\n\nThe finial winner is :\n");
    fprintf(fp,"\n\nThe finial winner is :\n");
    for (int i=0;i<Winner_num;i++){
        printf("%2d. Player %d",i+1,*(winner+i)+1);
        fprintf(fp,"%2d. Player %d",i+1,*(winner+i)+1);
    }

    printf("\n\nPress Enter to exit...\n");
    getchar();
    free(card);
    free(DiscardPile);
    return 0;
}
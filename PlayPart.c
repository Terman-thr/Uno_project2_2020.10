#include "Const.h"
#include "UI.h"
#include "time.h"
#include <stdlib.h>
#include "PlayPart.h"
#include <stdio.h>
#include "OperateCard.h"
#include "test.h"

void Play(int n,int c,int d,int r,int a_bool,FILE *fp){
    //-------------------------------------the card part----------------------------------------------------------------
    //create n players, clean their hands and initialize their scores
    Player *player1=Initialize(n);

    //init the stack, which are d desks of pokers
    int *card=(int *)calloc((unsigned int )d*52,sizeof(int));
    for (int i=1;i<=d;i++){
        for (int j=0;j<=51;j++)
            *(card+(i-1)*52+j)=j;
    }/* the card is now the pointer of the first 0 0-51->0-51*/

    //Shuffle the cards
    ShuffleCard(52 * d, card);
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
    int *DiscardPile=(int *)calloc((unsigned int )d*52,sizeof(int));
    for (int i=0;i<d*52;i++){
        *(DiscardPile+i)=-1;
    }/* the card is now the pointer of the first 0  all -1*/

    //Draw the first card and compare
    for (int i=0;i<n;i++)
        Dealer(card,Loop(i,player1));
    int first=FirstCardCmp(player1,n);/* first : the first player showing cards*/
    printf("\nDetermining the playing order...\n\n");
    fprintf(fp,"\nDetermining the playing order...\n\n");

    /* Show first cards in all mode*/
    for (int i=0;i<n;i++){
        printf("Player %d : ",i+1);
        fprintf(fp,"Player %d : ",i+1);
        Card2Str(Loop(i,player1)->card[0]);
        WCard2Str(Loop(i,player1)->card[0],fp);
        printf("\n");
        fprintf(fp,"\n");
    }
    printf("\nGame will start with player %d.\n\n\n",first+1);
    fprintf(fp,"\nGame will start with player %d.\n\n\n",first+1);

    //Create the play record and initialize it
    int Card_Record=-1,Card_valid_Rec=-1;

    //Discard the first card
    for (int i=0;i<n;i++){
        PlayACard(Loop(i,player1),0,&Card_Record,&Card_valid_Rec,DiscardPile);}

    //give each player c cards
    printf("Dealing cards...\n");
    fprintf(fp,"Dealing cards...\n");
    for (int i=0;i<n;i++){
        for (int j=0;j<c;j++){
            Dealer(card,Loop(i,player1));}
    }

    //sort players' cards
    for (int i=0;i<n;i++)
        bubble_sort((Loop(i,player1))->card,HandCardNum(Loop(i,player1)));

    /* show what players have in **demo** mode */
    if (a_bool){
        for (int i=0;i<n;i++){
            printf("\n Player %d:",i+1);
            fprintf(fp,"\n Player %d:",i+1);
            for (int j=0;j<c;j++){
                Card2Str(Loop(i,player1)->card[j]);
                WCard2Str(Loop(i,player1)->card[j],fp);
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
            printf("First card : \n");
            fprintf(fp,"First card : ");
            SingleCard2Image(Card_valid_Rec);
            printf("\n\n Game starts with Player %d\n", k_1 + 1);
            fprintf(fp,"\n\n Game starts with Player %d\n", k_1 + 1);
            printf("Dealing cards...\n");
            fprintf(fp,"Dealing cards...\n");

            int attack = 0, direction = 1;// 1 for clockwise 0 for counter-clockwise
            while (Loop(k_1,player1)->card[0] != -1) /* one player used all his/her cards */
            {
                //sort players' cards
                for (int i=0;i<n;i++)
                    bubble_sort((Loop(i,player1))->card,HandCardNum(Loop(i,player1)));

                k_1 = k;//adjust the k
                /* ------------------------------if there is no attack--------------------------- */
                if (attack == 0) {
                    for (SerialNum = 0; SerialNum < MAX_CARD + 1; SerialNum++) {
                        //ensure the card serial number is valid
                        if (SerialNum == MAX_CARD)/* the player have to draw a card */{
                            ShuffleDiscardPile(DiscardPile, card, d,fp);
                            Dealer(card, Loop(k,player1));
                            printf("\n\nPlayer %d draws: \n", k_1 + 1); //demo mode
                            fprintf(fp,"\n\nPlayer %d draws: ", k_1 + 1);

                            while ((Loop(k_1,player1))->card[t] != -1) { t++; }
                            SingleCard2Image((Loop(k_1,player1))->card[t - 1]);
                            WCard2Str((Loop(k_1,player1))->card[t - 1],fp);
                            t = 0;

                            printf("\nPlayer %d's card(s): \n", k_1 + 1);//show
                            fprintf(fp,"\nPlayer %d's card(s): ", k_1 + 1);
                            bubble_sort((Loop(k,player1))->card,HandCardNum(Loop(k_1,player1)));
                            Card2Ima(HandCardNum(Loop(k_1,player1)),(Loop(k_1,player1))->card);
                            while ((Loop(k_1,player1))->card[t] != -1) {
                                WCard2Str((Loop(k_1,player1))->card[t],fp);
                                printf("  ");
                                fprintf(fp,"  ");
                                t++;
                            }
                            t = 0;//reset the t;
                            break;
                        }
                        if (TestCard(&Card_valid_Rec, &(Loop(k,player1))->card[SerialNum])) /* fit rank or suit */
                        {
                            printf("\n\nPlayer %d plays: \n", k_1 + 1); //demo mode
                            fprintf(fp,"\n\nPlayer %d plays: \n", k_1 + 1);
                            SingleCard2Image((Loop(k,player1))->card[SerialNum]);
                            WCard2Str((Loop(k,player1))->card[SerialNum],fp);

                            switch ((Loop(k,player1))->card[SerialNum] % 13) /* test whether they are special cards */{
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
                            PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                            //show the cards
                            printf("\nPlayer %d's card(s): \n", k_1 + 1);//demo mode
                            fprintf(fp,"\nPlayer %d's card(s):", k_1 + 1);
                            Card2Ima(HandCardNum(Loop(k_1,player1)),(Loop(k_1,player1))->card);
                            while ((Loop(k_1,player1))->card[t] != -1)/* show */{
                                WCard2Str((Loop(k_1,player1))->card[t],fp);
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
                    Hand_poker_num = HandCardNum(Loop(k_1,player1));
                    for (SerialNum = 0; SerialNum < MAX_CARD + 1; SerialNum++) {
                        //ensure the card serial number is valid
                        if (SerialNum == MAX_CARD)/* player have no card to play i.e. draw many cards */{
                            for (int i = 0; i < attack; i++)/* draw card*/{
                                ShuffleDiscardPile(DiscardPile, card, d,fp);
                                Dealer(card, Loop(k_1,player1));
                            }
                            printf("\n\nPlayer %d draws :\n", k_1 + 1);
                            fprintf(fp,"\n\nPlayer %d draws :", k_1 + 1);
                            Card2Ima(attack,(Loop(k_1,player1))->card+Hand_poker_num);
                            for (t = 0; t < attack; t++) {
                                WCard2Str((Loop(k_1,player1))->card[Hand_poker_num + t],fp);
                                printf("   ");
                                fprintf(fp,"   ");
                            }
                            t = 0;

                            attack = 0;
                            printf("\nPlayer %d's card(s): \n", k_1 + 1);//show
                            fprintf(fp,"\nPlayer %d's card(s): ", k_1 + 1);
                            bubble_sort((Loop(k,player1))->card,HandCardNum(Loop(k_1,player1)));
                            Card2Ima(HandCardNum(Loop(k_1,player1)),(Loop(k_1,player1))->card);
                            while ((Loop(k_1,player1))->card[t] != -1) {
                                WCard2Str((Loop(k_1,player1))->card[t],fp);
                                printf("  ");
                                fprintf(fp,"  ");
                                t++;
                            }
                            t = 0;
                            break;
                        }

                        /* if he has special cards */
                        if (TestCard(&Card_valid_Rec, &(Loop(k,player1))->card[SerialNum])/* not fit rank or suit */
                            && ((Loop(k,player1))->card[SerialNum] % 13 == 5
                                || (Loop(k,player1))->card[SerialNum] % 13 == 0
                                || (Loop(k,player1))->card[SerialNum] % 13 == 1
                                || (Loop(k,player1))->card[SerialNum] % 13 == 9
                                || (Loop(k,player1))->card[SerialNum] % 13 == 10)) {
                            switch ((Loop(k,player1))->card[SerialNum] % 13) {
                                case 5: //7
                                    attack = 0;
                                    ShowPlayedCard(player1, SerialNum, k_1,fp);
                                    SingleCard2Image((Loop(k_1,player1))->card[SerialNum]);
                                    PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                    ShowHandCard(player1, k_1,fp);
                                    Card2Ima(HandCardNum(Loop(k_1,player1)),(Loop(k_1,player1))->card);
                                    break;
                                case 0: //2
                                    attack += 2;
                                    ShowPlayedCard(player1, SerialNum, k_1,fp);
                                    SingleCard2Image((Loop(k_1,player1))->card[SerialNum]);
                                    PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                    ShowHandCard(player1, k_1,fp);
                                    Card2Ima(HandCardNum(Loop(k_1,player1)),(Loop(k_1,player1))->card);
                                    break;
                                case 1: //3
                                    attack += 3;
                                    ShowPlayedCard(player1, SerialNum, k_1,fp);
                                    SingleCard2Image((Loop(k_1,player1))->card[SerialNum]);
                                    PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                    ShowHandCard(player1, k_1,fp);
                                    Card2Ima(HandCardNum(Loop(k_1,player1)),(Loop(k_1,player1))->card);
                                    break;
                                case 9: //J
                                    k = direction ? (k + 1) : (k - 1);
                                    ShowPlayedCard(player1, SerialNum, k_1,fp);
                                    SingleCard2Image((Loop(k_1,player1))->card[SerialNum]);
                                    PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                    ShowHandCard(player1, k_1,fp);
                                    Card2Ima(HandCardNum(Loop(k_1,player1)),(Loop(k_1,player1))->card);
                                    break;
                                case 10: //Q
                                    direction = !direction;
                                    ShowPlayedCard(player1, SerialNum, k_1,fp);
                                    SingleCard2Image((Loop(k_1,player1))->card[SerialNum]);
                                    PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                    ShowHandCard(player1, k_1,fp);
                                    Card2Ima(HandCardNum(Loop(k_1,player1)),(Loop(k_1,player1))->card);
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
            ShuffleCard(52 * d, card);
            ShuffleCard(52 * d, card);
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
                (Loop(i,player1))->score = (Loop(i,player1))->score - HandCardNum(Loop(i,player1));
                printf("Player %d :%d ,total: %d\n ", i + 1,- HandCardNum(Loop(i,player1)), (Loop(i,player1))->score);
                fprintf(fp,"Player %d :%d ,total: %d\n ", i + 1,- HandCardNum(Loop(i,player1)), (Loop(i,player1))->score);
            }
            printf("\n\nRound %d ends \n\n",r_1);
            fprintf(fp,"\n\nRound %d ends \n\n",r_1);

            //clean players' hand poker and re-deal cards
            for (int i = 0; i < n; i++)/* clean hands */{
                for (int j = 0; j < MAX_CARD; j++)
                    Loop(i,player1)->card[j] = -1;// -1>>no card
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
                        Dealer(card, (Loop(i,player1)));
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
            while ((Loop(k_1,player1))->card[0] != -1) /* one player used all his/her cards */
            {

                //sort players' cards
                for (int i=0;i<n;i++)
                    bubble_sort((Loop(i,player1))->card,HandCardNum(Loop(i,player1)));

                k_1 = k;//adjust the k
                printf("\nThe previous Card\n");
                SingleCard2Image(Card_valid_Rec);
                /* ------------------------------if there is no attack--------------------------- */
                if (attack == 0) {

                    //show player's card
                    printf("\n\nYour (Player %d's) card(s): \n", k_1 + 1);
                    fprintf(fp,"\n\nPlayer %d's card(s): \n", k_1 + 1);

                    Card2Ima(HandCardNum(Loop(k_1,player1)),(Loop(k_1,player1))->card);
                    while ((Loop(k_1,player1))->card[t] != -1) {
                        WCard2Str((Loop(k_1,player1))->card[t],fp);
                        fprintf(fp,"  ");
                        t++;
                    }
                    t = 0;//reset the t;

                    //Get valid input
                    SerialNum=GetValidInput(Card_valid_Rec,k,k_1,a_bool,player1,attack);

                    //ensure the card serial number is valid
                    if (SerialNum == -1)/* the player have to draw a card */{
                        ShuffleDiscardPile(DiscardPile, card, d,fp);
                        Dealer(card, Loop(k,player1));
                        printf("\n\nYou (Player %d) draws: \n", k_1 + 1);
                        fprintf(fp,"\n\nPlayer %d draws: \n", k_1 + 1);
                        //show card player draws
                        SingleCard2Image((Loop(k_1,player1))->card[HandCardNum(Loop(k_1,player1))-1]);
                        WCard2Str((Loop(k_1,player1))->card[HandCardNum(Loop(k_1,player1))-1],fp);

                        printf("\nPress Enter to continue\n");
                        getchar();
                        getchar();
                    }
                    else /* fit rank or suit */
                    {
                        printf("\n\nYou (Player %d) plays: \n", k_1 + 1); //demo mode
                        fprintf(fp,"\n\nPlayer %d plays: \n", k_1 + 1);
                        SingleCard2Image((Loop(k,player1))->card[SerialNum]);
                        WCard2Str((Loop(k,player1))->card[SerialNum],fp);
                        switch ((Loop(k,player1))->card[SerialNum] % 13) /* test whether they are special cards */{
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
                        PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
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
                    Hand_poker_num = HandCardNum(Loop(k_1,player1));

                    t=0;
                    printf("\nYour (Player %d's) card(s): \n", k_1 + 1);//show
                    fprintf(fp,"\nPlayer %d's card(s): \n", k_1 + 1);
                    Card2Ima(Hand_poker_num,(Loop(k_1,player1))->card);
                    while ((Loop(k_1,player1))->card[t] != -1) {
                        WCard2Str((Loop(k_1,player1))->card[t],fp);
                        fprintf(fp,"  ");
                        t++;
                    }

                    //Get valid input
                    SerialNum=GetValidInput(Card_valid_Rec,k,k_1,a_bool,player1,attack);

                    //ensure the card serial number is valid
                    if (SerialNum ==-1)/* player have no card to play i.e. draw many cards */{
                        for (int i = 0; i < attack; i++)/* draw card*/{
                            ShuffleDiscardPile(DiscardPile, card, d,fp);
                            Dealer(card, Loop(k_1,player1));
                        }
                        printf("\n\nYou (Player %d) draw :\n", k_1 + 1);
                        fprintf(fp,"\n\nPlayer %d draw :\n", k_1 + 1);
                        Card2Ima(attack,(Loop(k_1,player1))->card+Hand_poker_num);
                        for (int i=0;i<attack;i++) {
                            WCard2Str(*((Loop(k_1,player1))->card + Hand_poker_num+i), fp);
                        }
                        printf("\nPress Enter to continue\n");
                        getchar();
                        getchar();
                        attack = 0;
                    }
                        /* if he has special cards */
                    else{
                        switch ((Loop(k_1,player1))->card[SerialNum] % 13) {
                            case 5: //7
                                attack = 0;
                                printf("You played :\n");
                                fprintf(fp,"Player played :\n");
                                WCard2Str((Loop(k_1,player1))->card[SerialNum],fp);
                                SingleCard2Image((Loop(k_1,player1))->card[SerialNum]);
                                PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                break;
                            case 0: //2
                                attack += 2;
                                printf("You played :\n");
                                fprintf(fp,"Player played :\n");
                                WCard2Str((Loop(k_1,player1))->card[SerialNum],fp);
                                SingleCard2Image((Loop(k_1,player1))->card[SerialNum]);
                                PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                break;
                            case 1: //3
                                attack += 3;
                                printf("You played :\n");
                                fprintf(fp,"Player played :\n");
                                WCard2Str((Loop(k_1,player1))->card[SerialNum],fp);
                                SingleCard2Image((Loop(k_1,player1))->card[SerialNum]);
                                PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                break;
                            case 9: //J
                                k = direction ? (k + 1) : (k - 1);
                                printf("You played :\n");
                                fprintf(fp,"Player played :\n");
                                WCard2Str((Loop(k_1,player1))->card[SerialNum],fp);
                                SingleCard2Image((Loop(k_1,player1))->card[SerialNum]);
                                PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
                                break;
                            case 10: //Q
                                direction = !direction;
                                printf("You played :\n");
                                fprintf(fp,"Player played :\n");
                                WCard2Str((Loop(k_1,player1))->card[SerialNum],fp);
                                SingleCard2Image((Loop(k_1,player1))->card[SerialNum]);
                                PlayACard(Loop(k_1,player1), SerialNum, &Card_Record, &Card_valid_Rec, DiscardPile);
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
            ShuffleCard(52 * d, card);
            ShuffleCard(52 * d, card);

            for (int i = 0; i < d * 52; i++)/* clean discard pile */{
                DiscardPile[i] = -1;
            }
            printf("Preparing for another round...\n");
            printf("\n\nShuffling cards,please wait...\n\n\n");


            printf("\n---- Stats ----\nRound %d result:\n\n", r_1);
            fprintf(fp,"\n---- Stats ----\nRound %d result:\n\n", r_1);
            for (int i = 0; i < n; i++)/* calculate players score */{
                (Loop(i,player1))->score = (Loop(i,player1))->score - HandCardNum(Loop(i,player1));
                printf("Player %d :%d ,total: %d\n ", i + 1,- HandCardNum(Loop(i,player1)), (Loop(i,player1))->score);
                fprintf(fp,"Player %d :%d ,total: %d\n ", i + 1,- HandCardNum(Loop(i,player1)), (Loop(i,player1))->score);
            }
            printf("\n\nRound %d ends \n\n",r_1);
            fprintf(fp,"\n\nRound %d ends \n\n",r_1);

            //clean players' hand poker and re-deal cards
            for (int i = 0; i < n; i++)/* clean hands */{
                for (int j = 0; j < MAX_CARD; j++)
                    Loop(i,player1)->card[j] = -1;// -1>>no card
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
                        Dealer(card, (Loop(i,player1)));
                    }
                }
            }
            k = k_1;
            r--;
            r_1++;
        }
    }
    int Winner_num;
    int *winner=(int *)calloc((unsigned int )n,sizeof(int));
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
    DeletePlayer(player1,n);
}
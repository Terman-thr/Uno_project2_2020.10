#include <stdio.h>
#include <getopt.h>
#include <time.h>
#include "UI.h"
#include "OperateCard.h"
#include "test.h"
#include "PlayPart.h"
int str2num(const char *num) {
    int ans = 0, i = 0;
    while ((*(num + i)) != '\0') {
        ans = 10 * ans + *(num + i) - 48;
        i++;
    }
    return ans;
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
                    {"auto", no_argument, NULL, 'a'},
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

    if (c>=15 || c*n>=d*52){
        printf("You have draw too many cards.\n\n");
        return 0;
    }
    //define the file
    FILE *fp=fopen(filename,"w");
    printf("---- Initial setup ----\n Number of rounds: %d\n Number of decks: %d\n"
           " Number of players: %d\n Number of initial cards: %d\n File name of log :%s\n",r,d,n,c,filename);
    fprintf(fp,"---- Initial setup ----\n Number of rounds: %d\n Number of decks: %d\n"
           " Number of players: %d\n Number of initial cards: %d\n File name of log :%s\n",r,d,n,c,filename);

    Play(n,c,d,r,a_bool,fp);
    fclose(fp);
    return 0;
}
#include <stdio.h>
#include "UI.h"
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
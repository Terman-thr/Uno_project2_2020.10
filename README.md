# The game for One Card

## Program description

1.The main function is in main.c

2.When drawing the first card, if two players get card of same rank and suit, player with smaller serial number
will player first.

3.The Clean function is defined by :
   	#ifdef __WIN32
	#define CLEAR() do { system("cls"); } while ((void)0, 0)
	#else
	#define CLEAR() do { system("clear"); } while ((void)0, 0)
	#endif

## Bonus

Draw the cards using ASCII art.(I wrote two funcitons(included in UI.c/.h) to draw cards in ASCII Art.For higher efficiency, the ASCII art is only used in the player control mode and demo mode and onecard.log the cards are still string).


## Bug section

1.When you play a card in player mode, if you input characters, the program will lead to a infinite loop.





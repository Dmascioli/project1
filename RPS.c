/*
 * Darren Mascioli
 * COE 0449
 * ID: 4105961
 * Project 1, Part 1
 * Rock, Paper, Scissors
 */

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


char str_input[20]; /* input buffer */
char choices[3][10]; /* strings of choices */
int player_wins = 0; /* track player wins */
int comp_wins = 0; /* track computer wins */

void play_game() {
	
	char player_choice[10]; /* string of player's choice */
	int comp_choice; /* computer's choice */

	while(player_wins < 5 && comp_wins < 5) {
		printf("What is your choice? ");
		fgets(str_input, sizeof(str_input), stdin);
		sscanf(str_input, "%s", &player_choice);

		for(int i=0; i < 3; i++) {
			if(strcmp(player_choice, choices[i]) == 0){
				comp_choice = rand() % 3;

				if(comp_choice == i){
					printf("\nThe computer chooses %s. You tied this round.\n", choices[comp_choice]);
					break;
				}
				else if(comp_choice == 0 && i == 2){
					printf("\nThe computer chooses %s. You lose this round.\n", choices[comp_choice]);
					comp_wins++;
					break;
				}
				else if(i == 0 && comp_choice == 2){
					printf("\nThe computer chooses %s. You win this round!\n", choices[comp_choice]);
					player_wins++;
					break;
				}
				else if( i > comp_choice){	
					printf("\nThe computer chooses %s. You win this round!\n", choices[comp_choice]);
					player_wins++;
					break;
				}
				else {	
					printf("\nThe computer chooses %s. You lose this round.\n", choices[comp_choice]);
					comp_wins++;
					break;
				}
			}
		}

		printf("The score is now you: %d, computer: %d\n\n", player_wins, comp_wins);
	
	}
}



int main() {
	/* set choices array of strings */
	strcpy(choices[0], "rock");
	strcpy(choices[1], "paper");
	strcpy(choices[2], "scissors");

	/* seed random number */
	srand((unsigned int)time(NULL));


	printf("Welcome to Rock, Paper, Scissors!\n\n");
	char play_game_decision[10];
	for(;;) {
		printf("Would you like to play? ");
		fgets(str_input, sizeof(str_input), stdin);
		sscanf(str_input, "%s", &play_game_decision);

		if(!strcmp(play_game_decision, "yes")){	
			player_wins = 0;
			comp_wins = 0;
			play_game();
			
			if(player_wins == 5){
				printf("You win! Final score: You - %d, Computer - %d.\n", player_wins, comp_wins);
			}
			if(comp_wins == 5){	
				printf("You lose! Final score: You - %d, Computer - %d.\n", player_wins, comp_wins);
			}
		}
		else{break;}
	}
	return 0;

	

}

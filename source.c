#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define player_name_size 25
#define board_1_row 10
#define board_1_col 10

struct Player {
	int max_hp;
	int hp;
	char name[player_name_size];
	int attack;
	int row;
	int col;
};

struct Enemy {
	char type;
	int hp;
	int attack;
};

struct Field {
	char type;
	struct Player* player;
	struct Enemy* enemy;
};

struct Player* initiate_game();

char board1[board_1_row*board_1_col] = {
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
};

void show_menu(struct Field**);
char show_boards(struct Field**);
void initiate_board1(struct Field**);

char start_game(struct Player*, struct Field*);
void set_environment(struct Player*, struct Field*, int);
void random_unique_numbers(int*, int, int, int);
void print_board(struct Field*, int size);


int main() {
	srand(time(0));
	struct Player* player = initiate_game();
	struct Field* board = 0; 
	char score = 0;
	show_menu(&board);
	printf("Loading...");
	//score = start_game();
	switch (score) {
		case 1:
			printf("You have defeated all the enemies!");
			break;
		default:
			printf("You lost! Better luck next time!");
			break;
	}

	if (player != 0) {
		free(player);
	}

	if (board != 0) {
		free(board);
	}

	printf("Thanks for playing :)");
}

// TODO: algorithm to generate a list of random unique intergers from 0 to given N.
void random_unique_numbers(int* num_arr, int size, int n) {
	int i = 0;
	while (i < size) {

	}
}

void set_environment(struct Player* player, struct Field* board, int board_size) {
	int i;
	// TODO: generate enemies positions randomly.
	// ... Enemies amount is fixed and must be respected.
	// ... every position must be unique, otherwise field will point only to one of the stacked
	// ... decreasing amount of enemies player can fight, which causes him to be stuck thanks to
	// ... enemies count.
	int positions[5] = {};

	for (i = 0; i < board_size; i++) {
		//(board+1)->enemy
	}
}

char start_game(struct Player* player, struct Field* board) {
	char score = -1;
	int i;
	do {
		for(i = 0; i < board_1_col*board_1_row; i++)


		system("CLS");
	} while (score == -1);

	return 0;
}

void initiate_board1(struct Field** board_pointer) {
	int i = 0, row_n, col_n; 
	int board_size = board_1_row * board_1_col;
	struct Field* new_board = (struct Field*)malloc(board_size * sizeof(struct Field));
	if (new_board != 0) {
		for (i = 0; i < board_1_row; i++) {
			row_n = i / board_1_col;
			col_n = i % board_1_row;
			(new_board + i)->type = board1[i];
			(new_board + i)->player = 0;
			(new_board + i)->enemy = 0;
		}
	}
	*board_pointer = new_board;
}

char show_boards(struct Field** board_pointer) {
	system("CLS");
	char current_choice = 1;
	char picked_choice = 0;
	char max_choice = 2;
	char min_choice = 1;
	char key;
	do {
		
		// Boards picker menu layout
		printf("===== MENU - board picker =====\n\n");
		printf("PICK ONE OF FOLLOWING: \n\n");
		if (current_choice == 1) {
			printf("->");
		}
		printf("1. Go back to menu.\n");
		if (current_choice == 2) {
			printf("->");
		}
		printf("2. board1 (%d x %d)\n", board_1_row, board_1_col);

		// User input handler
		key = getch();
		switch (key) {

			// W or S to navigate up or down the menu list.
			case 'w':
			case 'W':
				if (current_choice > min_choice)
					current_choice--;
				break;
			case 's':
			case 'S':
				if (current_choice < max_choice)
					current_choice++;
				break;

			// Enter to confirm a choice.
			case 13:
				picked_choice = current_choice;
				break;
		}
		system("CLS");
		// If any choice is made, choice "window" is closed.
	} while (picked_choice == 0);

	// Choice is handled outside the "window"
	// because all of them will close it.
	switch (picked_choice) {
		case 1:
			break;
		case 2: 
			// allocates board1 layout to memory place where "board" pointer points.
			initiate_board1(board_pointer);
			// if board is successfully created returns 1
			// indicates game can be started and main menu will end.
			return 1;
	}
	
	// board picker left without starting the game (no board picked or pick failed).
	return 0;
}

void show_menu(struct Field** board_pointer) {
	system("CLS");
	char current_option = 1;
	char submit_option = 0;
	char key = 0;
	char start_game = 0;
	do {

		printf("Start of znak: ");
		printf("%d", key);
		printf("End of znak.\n\n");
		submit_option = 0;

		// Menu layout in console
		printf("===== MENU =====\n\n");
		if (current_option == 1) {
			printf("->");
		}
		printf("1. Choose board.\n");
		if (current_option == 2) {
			printf("->");
		}
		printf("2. Exit.\n");
		key = getch();

		// Key handler

		switch (key) {
			// W and S to navigate up and down the menu list.
			case 's':
			case 'S':
				if (current_option < 2) {
					current_option++;
				}
				break;
			case 'w':
			case 'W':
				if (current_option > 1) {
					current_option--;
				}
				break;
			// Enter to confirm the choice.
			// Wartoœæ, któr¹ wyci¹ga getch() po naciœniêciu ENTER z bufera klawiatury.
			case 13:
				submit_option = current_option;
				break;
		}

		// Confirmed choice handled inside the loop, because
		// this choice window will nest other choice windows
		// and leaving this window must be confirmed by choosing option 2
		if (submit_option != 0) {
			switch (submit_option) {
				case 1: 
					// another menu that returns whether the game can be started or not.
					// 1 is only returned if player successfully chooses a board.
					start_game = show_boards(board_pointer);
					break;
				case 2: 
					break;
			}
		}
		system("CLS");
	// Menu continues until either player exits or fulfill game start condition.
	} while (submit_option != 2 && start_game == 0);
	system("CLS");
}

struct Player* initiate_game() {
	system("CLS");
	char name[player_name_size];
	do {
		printf("WELCOME IN POLAND! A LOVELY PLACE TO LIVE.\n\n");
		printf("In order to start you must enter your name: ");
		scanf_s("%s", name, player_name_size);
	} while (0);
	system("CLS");

	struct Player* new_player = (struct Player*) malloc(sizeof(struct Player));
	strcpy_s(&new_player->name, player_name_size, name);
	new_player->hp = 100;
	new_player->max_hp = 100;
	new_player->attack = 5;
	new_player->row = 0;
	new_player->col = 0;
	return new_player;
}

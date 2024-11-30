#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <math.h>
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
	int row;
	int col;
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

void list_navigator(char*, char*, int, int);

void show_menu(struct Field**, int*, int*);
char show_boards(struct Field**, int*, int*);
void initiate_board1(struct Field**, int*, int*);

char start_game(struct Player*, struct Field*, int, int);
void set_player(struct Field*, struct Player*);                               
struct Enemy* set_enemies(struct Field*, int, int, int);
void print_board(struct Field*, int, int);
struct Enemy* move_up(struct Player*, struct Field*, int);
struct Enemy* move_down(struct Player*, struct Field*, int, int);
struct Enemy* move_left(struct Player*, struct Field*, int);
struct Enemy* move_right(struct Player*, struct Field*, int, int);
char encounter_goblin(struct Player*, struct Enemy*);
char encounter_cyclops(struct Player*, struct Enemy*);
char clean_enemy(struct Field*, struct Enemy*, int);
void random_unique_numbers(int*, int, int, int);


int main() {
	setlocale(LC_CTYPE, "Polish");
	srand(time(0));
	int r = 0, k = 0;
	struct Player* player = initiate_game();
	if (player == 0) {
		return 1;
	}
	struct Field* board = 0; 
	char score = 0;
	show_menu(&board, &r, &k);
	if (board == 0) {
		return 1;
	}
	score = start_game(player, board, r, k);
	switch (score) {
		case 1:
			printf("You died!\n\n");
			break;
		case 2: 
			printf("You won! Congratulations!!!\n\n");
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

void set_player(struct Field* board, struct Player* player) {
	if (player == 0)
		return -1;
	player->max_hp = 10000;
	player->hp = 10000;
	player->attack = 25;
	player->row = 0;
	player->col = 0;
	board->player = player;
}

struct Enemy* set_enemies(struct Field* board, int r, int k, int enemy_counter) {

	// TODO: generate enemies positions randomly.
	// ... Enemies amount is fixed and must be respected.
	// ... every position must be unique, otherwise field will point only to one of the stacked
	// ... decreasing amount of enemies player can fight, which causes him to be stuck thanks to
	// ... enemies count.
	// uncomment when positions are unique random numbers, not set by author
	/*
	int* positions = (int*)malloc(enemy_counter * sizeof(int));
	if (positions == 0)
		return -1;
	*/
	int i;
	int position[5] = { 5, 20, 50, 69, 76 };
	char type = 'A';
	struct Enemy* enemies = (struct Enemy*)malloc(enemy_counter * sizeof(struct Enemy));
	if (enemies == 0)
		return 0;
	for (i = 0; i < enemy_counter; i++) {

		(enemies + i)->type = type;
		printf("row: %d col: %d\n", ((enemies + i)->row = position[i] / k), ((enemies + i)->col = position[i] % k));
		(board + position[i])->enemy = enemies + i;
		if (i == 1)
			type = 'B';
	}
	return enemies;
}

/*
	r = 3, k = 5
	max_wykonan r * k  = 15
	nr_iteracji = 15
	while nr_iteracji <= 15:
	n_r = 2, n_k = 3
	position = n_r * 5 + n_k = 13

	                
	D D D D D 
	             
	D D D D D
	      v
	D D D D D

*/

void print_board(struct Field* board, int r, int k) {
	int n_r, n_k;
	struct Enemy* enemy;
	struct Player* player;
	for (n_r = 0; n_r < r; n_r++) {
		for (n_k = 0; n_k < k; n_k++) {
			enemy = (board + n_r * k + n_k)->enemy;
			player = (board + n_r * k + n_k)->player;
			printf("[");
			if (enemy != 0 && player != 0) {
				printf(" E ");
			}
			else if (enemy != 0) {
				printf("%3c", enemy->type);
			}
			else if (player != 0) {
				printf(" P ");
			}
			else {
				printf("   ");
			}
			
			printf("]");
		}
		printf("\n");
	}
}

struct Enemy* move_up(struct Player* player, struct Field* board, int k) {
	if (player->row - 1 >= 0) {
		(board + player->row * k + player->col)->player = 0;
		player->row = player->row - 1;
		(board + player->row * k + player->col)->player = player;
	}
	return (board + player->row * k + player->col)->enemy;
}

struct Enemy* move_down(struct Player* player, struct Field* board, int r, int k) {
	if (player->row + 1 < r) {
		(board + player->row * k + player->col)->player = 0;
		player->row = player->row + 1;
		(board + player->row * k + player->col)->player = player;
	}
	return (board + player->row * k + player->col)->enemy;
}

struct Enemy* move_left(struct Player* player, struct Field* board, int k) {
	if (player->col - 1 >= 0) {
		(board + player->row * k + player->col)->player = 0;
		player->col = player->col - 1;
		(board + player->row * k + player->col)->player = player;
	}
	return (board + player->row * k + player->col)->enemy;
}

struct Enemy* move_right(struct Player* player, struct Field* board, int r, int k) {
	if (player->col + 1 < k) {
		(board + player->row * k + player->col)->player = 0;
		player->col = player->col + 1;
		(board + player->row * k + player->col)->player = player;
	}
	return (board + player->row * k + player->col)->enemy;
}

char encounter_goblin(struct Player* player, struct Enemy* enemy) {
	char outcome = -1;
	int g_hp = 150, g_max_hp = 150, g_attack = 10;
	char picked_choice = 1, commited_choice = 0, key;
	printf("You encounter a goblin. You begin fight. \n\n");
	do {
		do {
			commited_choice = 0;
			printf("Status: \n");
			printf("hp: %d/%d, attack: %d\n\n", player->hp, player->max_hp, player->attack);
			printf("Enemy status: ");
			printf("hp: %d/%d, attack: %d\n\n", g_hp, g_max_hp, g_attack);
			printf("What do you want to do?\n\n");
			if (picked_choice == 1) {
				printf("->");
			}
			printf("1. Attack\n");
			if (picked_choice == 2) {
				printf("->");
			}
			printf("2. Defend (TODO)\n");
			list_navigator(&picked_choice, &commited_choice, 1, 2);
		} while (commited_choice == 0);

		system("CLS");

		switch (commited_choice) {
			case 1:
				g_hp -= player->attack;
				break;
			case 2:
				break;
		}

		if (g_hp <= 0) {
			outcome = 1;
			break;
		}else{
			printf("Player deals %d to Goblin.\n", g_attack);
		}

		player->hp -= g_attack;
		if (player->hp <= 0) {
			outcome = 0;
			break;
		}
		else {
			printf("Goblin deals %d damage!\n", g_attack);
		}
	} while (outcome == -1);
	return outcome;
}

char encounter_cyclops(struct Player* player, struct Enemy* enemy) {
	char outcome = -1;
	int cycl_hp = 500, cycl_max_hp = 500, cycl_attack = 50;
	int min = 0, max = 5, rand_dmg;
	char picked_choice = 1, commited_choice = 0, key;
	printf("You encounter cyclops. You begin fight. \n\n");
	do {
		do {
			commited_choice = 0;
			printf("Status: \n");
			printf("hp: %d/%d, attack: %d\n\n", player->hp, player->max_hp, player->attack);
			printf("Cyclops status: ");
			printf("hp: %d/%d, attack: %d\n\n", cycl_hp, cycl_max_hp, cycl_attack);
			printf("What do you want to do?\n\n");
			if (picked_choice == 1) {
				printf("->");
			}
			printf("1. Attack\n");
			if (picked_choice == 2) {
				printf("->");
			}
			printf("2. Defend (TODO)\n");
			list_navigator(&picked_choice, &commited_choice, 1, 2);
		} while (commited_choice == 0);

		system("CLS");

		switch (commited_choice) {
		case 1:
			cycl_hp -= player->attack;
			printf("Player deals %d to Cyclops.\n", cycl_attack);
			break;
		case 2:
			break;
		}

		if (cycl_hp <= 0) {
			outcome = 1;
			printf("Cyclops defeated!\n");
			break;
		}

		rand_dmg = rand() % (abs(max - min) + 1) + min;
		if (rand_dmg == 5) {
			player->hp -= cycl_attack;
			printf("Cyclops hits! Critical dmg %d dealt to player!\n", cycl_attack);
		}
		else {
			player->hp -= rand_dmg;
			printf("Cyclops hits ground. Deals %d dmg to player.\n", rand_dmg);
		}

		if (player->hp <= 0) {
			outcome = 0;
			break;
		}
	} while (outcome == -1);
	return outcome;
}

char clean_enemy(struct Field* board, struct Enemy* enemy, int k) {
	(board + (enemy->row) * k + enemy->col)->enemy = 0;
}

char start_game(struct Player* player, struct Field* board, int r, int k) {
	// Reason game is over - not over -1 by default.
	char reason = -1, player_input, outcome_enc = -1;
	int enemy_counter = 5, board_size = r * k;
	struct Enemy* enemies = 0;
	struct Enemy* encounter = 0;
	set_player(board, player);
	enemies = set_enemies(board, r, k, enemy_counter);
	if (enemies == 0) {
		return -1;
	}

	do {
		printf("Board: \n\n");
		print_board(board, r, k);
		printf("\n\n");

		printf("W, A, S, D - zmień swoją pozycję na planszy.\n");
		printf("E - użyj przedmiotu.\n\n");
		player_input = getch();
		system("CLS");
		switch (player_input) {
			case 'w':
			case 'W':
				encounter = move_up(player, board, k);
				break;
			case 's':
			case 'S':
				encounter = move_down(player, board, r, k);
				break;
			case 'a':
			case 'A':
				encounter = move_left(player, board, k);
				break;
			case 'd':
			case 'D':
				encounter = move_right(player, board, r, k);
				break;
		}

		if (encounter != 0) {
			switch (encounter->type) {
				case 'A':
					outcome_enc = encounter_goblin(player, encounter);
					break;
				case 'B':
					outcome_enc = encounter_cyclops(player, encounter);
					break;
			}

			switch (outcome_enc) {
				case 0:
					reason = 0;
					break;
				case 1:
					clean_enemy(board, encounter, k);
					enemy_counter--;
					break;
			}
		}

		if (enemy_counter <= 0) {
			reason = 2;
		}

		encounter = 0;
		outcome_enc = -1;
	} while (reason == -1);

	if (enemies != 0) {
		free(enemies);
	}

	system("CLS");
	return reason;
}

void initiate_board1(struct Field** board_pointer, int*r, int*k) {
	int i, j, n; 
	int board_size = board_1_row * board_1_col;
	struct Field* new_board = (struct Field*)malloc(board_size * sizeof(struct Field));
	if (new_board != 0) {
		*r = board_1_row;
		*k = board_1_col;
		for (i = 0; i < board_1_row; i++) {
			for (j = 0; j < board_1_col; j++) {
				n = i * board_1_col + j;
				(new_board + n)->type = board1[i];
				(new_board + n)->player = 0;
				(new_board + n)->enemy = 0;
			}
		}
	}
	*board_pointer = new_board;
}

char show_boards(struct Field** board_pointer, int*r, int*k) {
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
		list_navigator(&current_choice, &picked_choice, 1, 2);
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
			initiate_board1(board_pointer, r, k);
			// if board is successfully created returns 1
			// indicates game can be started and main menu will end.
			return 1;
	}
	
	// board picker left without starting the game (no board picked or pick failed).
	return 0;
}

void show_menu(struct Field** board_pointer, int*r, int*k) {
	system("CLS");
	char current_option = 1;
	char submit_option = 0;
	char key = 0;
	char start_game = 0;
	do {
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
		list_navigator(&current_option, &submit_option, 1, 2);

		// Confirmed choice handled inside the loop, because
		// this choice window will nest other choice windows
		// and leaving this window must be confirmed by choosing option 2
		if (submit_option != 0) {
			switch (submit_option) {
				case 1: 
					// another menu that returns whether the game can be started or not.
					// 1 is only returned if player successfully chooses a board.
					start_game = show_boards(board_pointer, r, k);
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
	if (new_player == 0) {
		return -0;
	}
	strcpy_s(&new_player->name, player_name_size, name);
	return new_player;
}

void list_navigator(char* pick, char* cmted, int min, int max) {
	char key;
	key = getch();
	switch (key) {
		case 'w':
		case 'W':
			if (*pick - 1 >= min) {
				*pick = *pick - 1;
			}
			break;
		case 's':
		case 'S':
			if (*pick + 1 <= max) {
				*pick = *pick + 1;
			}
			break;
		case 13:
			*cmted = *pick;
			break;
	}
}

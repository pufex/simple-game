#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <math.h>
#define player_name_size 25
#define item_name_size 30
#define board_1_row 10
#define board_1_col 10
#define items_amount 3

struct inv_item {
	// items effect is based on the type index
	int index;
	char name[item_name_size];
	// items can be used only if they're available in the inventory.
	int amount;
	int max_amount;
};

struct board_item {
	int index;
	char name[item_name_size];
	int amount;
	int row;
	int col;
};

struct Enemy_enc {
	char type;
	int hp;
	int hp_max;
	int attack;
};

struct Player {
	int max_hp;
	int hp;
	char name[player_name_size];
	int attack;
	int row;
	int col;
	struct inv_item* inventory;

	// encounter only members
	char used_defend;
};

struct Enemy {
	char type;
	int row;
	int col;
};

struct Field {
	struct Player* player;
	struct Enemy* enemy;
	struct board_item* item;
};


// ======= few app setup functions

struct inv_item* initiate_inventory();
struct Player* initiate_player(struct inv_item*);


// main menu
void show_menu(struct Field**, int*, int*);
// boards menu inside main menu
// ... uses initiate a board functions
char show_boards(struct Field**, int*, int*);


// TODO: make it so that each board is stored as data in a file 
// ... and initiated according to their parameters automatically
// ... on player choice,
// ... instead of manually writing seperate functions that
// ... initiate each board
// initiates board1
void initiate_board1(struct Field**, int*, int*);

// game setup
char start_game(struct Player*, struct Field*, int, int);
void set_player(struct Field*, struct Player*);                               
struct Enemy* set_enemies(struct Field*, int, int, int);
struct board_item* set_items(struct Field*, int, int);

// game layout
void print_player_status(struct Player*);
void print_board(struct Field*, int, int);
void inventory_menu(struct Player*);
void item_menu(struct Player*, int);

// controls around board
// all update players position in his struct and on the board

void move_up(struct Player*, struct Field*, int);
void move_down(struct Player*, struct Field*, int, int);
void move_left(struct Player*, struct Field*, int);
void move_right(struct Player*, struct Field*, int, int);

// inventory items functions
char use_item(struct Player*, int);
char remove_item(struct inv_item*, int);
int give_item(struct inv_item*, int, int);

// cleans an item from the board
void clean_item(struct Field*, struct board_item*, int);


// TODO: create a generic encounter function which returns encounter result,
// ... and wraps all the logic in a single function.

char start_encounter(struct Player*, struct Enemy*);
void print_enemy_enc_status(struct Enemy_enc*);
void print_enc_options(int);
void setup_enc_enemy(struct Enemy_enc*);

// enemy type specific encounter behavior functions
void goblin_behavior(struct Player*, struct Enemy_enc*);
void cyclops_behavior(struct Player*, struct Enemy_enc*);


// removes defeated enemy from the board
void clean_enemy(struct Field*, struct Enemy*, int);

// utils                     
void random_unique_numbers(int*, int, int, int);
void list_navigator(char*, char*, int, int);
int rand_min_max(int, int);
void switch_ints(int*, int*);

int main() {
	setlocale(LC_CTYPE, "Polish");
	srand(time(0));

	int r = 0, k = 0;
	struct inv_item* inventory = initiate_inventory();
	if (inventory == 0) {
		return 1;
	}
	// allocates memory for struct Player and returns pointer to that memory.
	struct Player* player = initiate_player(inventory);
	if (player == 0) {
		return 1;
	}
	
	// pointer to the start of the board, i.e. array of board cells
	struct Field* board = 0; 
	char score = 0;
	// this here's purpose is to assign r and k with board column and row lenghts
	// ...and pointer to the board array, so that it can be used in game.
	show_menu(&board, &r, &k);
	if (board == 0) {
		return 1;
	}
	// start game and return result (win or lose).
	score = start_game(player, board, r, k);
	// print information to the player how the game ended.
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

	// frees memory allocated for the player struct and board array.
	
	if (inventory != 0) {
		free(inventory);
	}
	
	if (player != 0) {
		free(player);
	}

	if (board != 0) {
		free(board);
	}

	printf("Thanks for playing :)");
}

struct inv_item* initiate_inventory() {
	int i;
	struct inv_item* inventory = (struct inv_item*)malloc(items_amount * sizeof(struct inv_item));
	if (inventory == 0) {
		return 0;
	}
	for (i = 0; i < items_amount; i++) {
		(inventory + i)->index = i;
		(inventory + i)->amount = 0;
		(inventory + i)->max_amount = 10;
		switch (i) {
			case 0:
				(inventory + i)->amount = 1;
				(inventory + i)->max_amount = 100;
				strcpy_s(&(inventory + i)->name, item_name_size, "Mikstura HP (M)");
				break;
			case 1:
				(inventory + i)->max_amount = 100;
				strcpy_s(&(inventory + i)->name, item_name_size, "Mikstura HP (S)");
				break;
			case 2:
				(inventory + i)->max_amount = 100;
				strcpy_s(&(inventory + i)->name, item_name_size, "Mikstura HP (D)");
				break;
		}
	}
	return inventory;
}

struct Player* initiate_player(struct inv_item* inventory) {
	system("CLS");
	char name[player_name_size];
	do {
		printf("WELCOME IN POLAND! A LOVELY PLACE TO LIVE.\n\n");
		printf("In order to start you must enter your name: ");
		scanf_s("%s", name, player_name_size);
	} while (0);
	system("CLS");

	struct Player* new_player = (struct Player*)malloc(sizeof(struct Player));
	if (new_player == 0) {
		return 0;
	}
	new_player->inventory = inventory;
	strcpy_s(&new_player->name, player_name_size, name);
	return new_player;
}

void show_menu(struct Field** board_pointer, int* r, int* k) {
	system("CLS");
	char current_option = 1;
	char submit_option = 0;
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

char show_boards(struct Field** board_pointer, int* r, int* k) {
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


void initiate_board1(struct Field** board_pointer, int* r, int* k) {
	int i, j, n;
	int board_size = board_1_row * board_1_col;
	struct Field* new_board = (struct Field*)malloc(board_size * sizeof(struct Field));
	if (new_board != 0) {
		*r = board_1_row;
		*k = board_1_col;
		for (i = 0; i < board_1_row; i++) {
			for (j = 0; j < board_1_col; j++) {
				n = i * board_1_col + j;
				(new_board + n)->player = 0;
				(new_board + n)->enemy = 0;
				(new_board + n)->item = 0;
			}
		}
	}
	*board_pointer = new_board;
}

/*

Game loop scenarios:

Scenarios:
	Game ending scenarios:
		'W' - player has successfully killed all enemies (loss).
		'L' - player has been defeated by an enemy (win).
	Encounter scenarios:
		'E' - player moved and encountered enemy on their path.
		'I' - player moved and encountered an item on their path.

Each iteration of the do ... while loop allows player to commit an action.
Actions include:

 - Using items
 - Moving around the map 

Using items on map affects players status which include his health, max health, and attack.
It does not cause any scenario to occur.

Moving around the map can cause all of the above scenarios. Order of priority of scenarios other than game ending are as follows:

	if E:
		return W or L
	if I:
		update inventory

So first, based on player's position, it's checked whether an enemy was encountered. If so, then initiate encounter based on enemy's type.
Then, whether or not an enemy was encountered, checks based on player's current position if an item was encountered. Updates inventory based on item's code.


*/

char start_game(struct Player* player, struct Field* board, int r, int k) {
	// Reason game is over - not over -1 by default.
	char reason = -1, player_input;
	int enemy_counter = 5, board_size = r * k, remaining_amount = 0;
	struct Enemy* enemies = 0;
	struct board_item* items = 0;
	struct Enemy* enc_enemy = 0;
	struct board_item* enc_item = 0;
	int outcome_enc;
	set_player(board, player);
	enemies = set_enemies(board, r, k, enemy_counter);
	if (enemies == 0) {
		return -1;
	}
	items = set_items(board, r, k);
	if (items == 0) {
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
				move_up(player, board, k);
				break;
			case 's':
			case 'S':
				move_down(player, board, r, k);
				break;
			case 'a':
			case 'A':
				move_left(player, board, k);
				break;
			case 'd':
			case 'D':
				move_right(player, board, r, k);
				break;
			case 'e':
			case 'E':
				inventory_menu(player);
				break;
		}

		// Check if player has encountered an enemy:
		enc_enemy = (board + (player->row) * k + player->col)->enemy;
		if (enc_enemy) { // if so, start encounter based on enemy type
			outcome_enc = start_encounter(player, enc_enemy);
			switch (outcome_enc) {
				case 0:
					reason = 0;
					break;
				case 1:
					clean_enemy(board, enc_enemy, k);
					break;
			}
		}

		if (reason == 0) {
			break;
		}
	
		// TODO: Check if player has encountered an item:
		enc_item = (board + (player->row) * k + player->col)->item;
		if (enc_item != 0) {
			printf("You found %d %s (item index: %d)!\n\n", enc_item->amount, enc_item->name, enc_item->index);
			remaining_amount = give_item(player->inventory, enc_item->index, enc_item->amount);
			if (remaining_amount == enc_item->amount) {
				printf("Unfortunately, you can't fit these items into your inventory.\n");
				printf("You will have to pick them up when you have some space.\n\n");
			}
			else if (remaining_amount > 0) {
				printf("You could only take %d with you.\n", enc_item->amount - remaining_amount);
				printf("You can pick them up later when you have more space.\n\n");
			}
			else {
				clean_item(board, enc_item, k);
			}
			enc_item->amount = remaining_amount;
		}

		if (enemy_counter <= 0) {
			reason = 2;
		}
	} while (reason == -1);

	if (enemies != 0) {
		free(enemies);
	}

	if (items != 0) {
		free(items);
	}

	system("CLS");
	return reason;
}

void set_player(struct Field* board, struct Player* player) {
	if (player == 0)
		return -1;
	// Set up players stats
	player->max_hp = 200;
	player->hp = 200;
	player->attack = 25;

	// Set up default battle parameters
	player->used_defend = 0;

	// Place player in the start of the board.
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
	if (enemies == 0) {
		return 0;
	}
	for (i = 0; i < enemy_counter; i++) {
		(enemies + i)->type = type; // gives each enemy their type (influences their behavior).
		(enemies + i)->row = position[i] / k;
		(enemies + i)->col = position[i] % k;
		(board + position[i])->enemy = enemies + i; // places each enemy on their respective position in board.
		if (i == 1)
			type = 'B';
	}
	return enemies; // returns enemies address, because memory was allocated dynamically.
}

struct board_item* set_items(struct Field* board, int r, int k) {
	int i, items_count = 5, amount, index;
	int position[5] = { 2, 5, 20, 30, 69 };
	// TODO: same as with enemies - generate positions as unique random numbers between 0 and r x k.
	// For now, it's set by author.
	struct board_item* items = (struct board_item*)malloc(items_count * sizeof(struct board_item));
	if (items == 0) {
		return 0;
	}
	for (i = 0; i < items_count; i++) {
		(board + position[i])->item = items + i;
		amount = rand_min_max(1, 5);
		index = rand_min_max(0, items_amount - 1);
		(items + i)->index = index;
		(items + i)->amount = amount;
		strcpy_s(&((items + i)->name), item_name_size, "placeholder");
		(items + i)->row = position[i] / k;
		(items + i)->col = position[i] % k;
	}
	return items;
}

void print_player_status(struct Player* player) {
	printf("Player status: \n");
	printf("HP: %d/%d, ATK: %d\n\n", player->hp, player->max_hp, player->attack);
}

void print_board(struct Field* board, int r, int k) {
	int n_r, n_k;
	struct Enemy* enemy;
	struct Player* player;
	// iterates over each cell in the board to display it
	// in correct manner. Each cell is [] with 3 char space inside.
	for (n_r = 0; n_r < r; n_r++) {
		for (n_k = 0; n_k < k; n_k++) {
			// gets both enemy and player pointer from board cell
			// ... if player pointer is 0, it means players is not in the cell.
			// ... same for enemy
			enemy = (board + n_r * k + n_k)->enemy;
			player = (board + n_r * k + n_k)->player;
			printf("[");
			// both players are in the cell
			if (enemy != 0 && player != 0) {
				printf(" E ");
			}

			// enemy's there (can't have both, since it would satisfy previous condition)
			else if (enemy != 0) {
				printf("%3c", enemy->type);
			}
			// then check if player is instead.
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

void inventory_menu(struct Player* player) {
	char picked_option = 1, commited_option = -1;
	struct inv_item* inventory = player->inventory;
	int i; 
	do {

		// Inventory menu Layout
		commited_option = -1;
		print_player_status(player);
		printf("Wybierz przedmiot: \n\n");

		if (picked_option == 0) {
			printf("->");
		}
;		printf("0. Powrót \n\n");
		for (i = 0;i < items_amount; i++) {
			if (picked_option == i+1) {
				printf("->");
			}
			printf("%d. %s\n", i+1, (inventory+i)->name);
		}

		// Option list navigation functionality
		list_navigator(&picked_option, &commited_option, 0, items_amount);
		system("CLS");

		// If player commits option by pressing ENTER:
		switch (commited_option) {
			
			// Either wasn't commited or player decides to leave.
			case -1:
			case 0: 
				break;
			

			// An item was selected.
			default:
				item_menu(player, commited_option - 1);
				break;
		}
	} while (commited_option != 0);
}

void item_menu(struct Player* player, int index) {
	char picked_option = 1, commited_option, success = 0;
	int i;
	struct inv_item* inventory = player->inventory;
	do {
		commited_option = -1;
		// Layout
		print_player_status(player);
		printf("%s - ilość: %d\n\n", (inventory + index)->name, (inventory + index)->amount);
		if (picked_option == 0) {
			printf("->");
		}
		printf("0. Powrót \n\n");
		if (picked_option == 1) {
			printf("->");
		}
		printf("1. Uzyj przedmiotu (-1).\n");

		if (picked_option == 2) {
			printf("->");
		}
		printf("2. Usuń przedmiot (-1).\n");
		printf("\n");

		// List navigation functionality.
		list_navigator(&picked_option, &commited_option, 0, items_amount);
		system("CLS");

		switch (commited_option) {
			case 0: break;

			// Use item option
			case 1:
				// Function is based around they inventory array
				// ... so it automatically detects if player has 0 amount
				// ... of item under giver index.
				success = use_item(player, index);
				if (success == 0) {
					printf("You can't use item that you don't have. \n\n");
				}
				break;
			// Remove 1 item from inventory
			case 2:
				// Obviously, function will not have player have negative
				// ... amount of items.
				success = remove_item(inventory, index);
				if (success == 0) {
					printf("You can't remove items that you don't have.\n\n");
				}
				break;
		}
	} while (commited_option != 0);
}

void move_up(struct Player* player, struct Field* board, int k) {
	if (player->row - 1 >= 0) { // keeps player in the board.
		// clears player in previous cell, then updates player position
		// both in player struct and board cell.
		(board + player->row * k + player->col)->player = 0;
		player->row = player->row - 1;
		(board + player->row * k + player->col)->player = player;
	}
}

void move_down(struct Player* player, struct Field* board, int r, int k) {
	if (player->row + 1 < r) { // keeps player in the board.
		// clears player in previous cell, then updates player position
		// both in player struct and board cell.
		(board + player->row * k + player->col)->player = 0;
		player->row = player->row + 1;
		(board + player->row * k + player->col)->player = player;
	}
}

void move_left(struct Player* player, struct Field* board, int k) {
	if (player->col - 1 >= 0) { // keeps player in the board.
		// clears player in previous cell, then updates player position
		// both in player struct and board cell.
		(board + player->row * k + player->col)->player = 0;
		player->col = player->col - 1;
		(board + player->row * k + player->col)->player = player;
	}
}

void move_right(struct Player* player, struct Field* board, int r, int k) {
	if (player->col + 1 < k) { // keeps player in the board.
		// clears player in previous cell, then updates player position
		// both in player struct and board cell.
		(board + player->row * k + player->col)->player = 0;
		player->col = player->col + 1;
		(board + player->row * k + player->col)->player = player;
	}
}

char use_item(struct Player* player, int index) {
	// Function that uses item from the inventory of given index.
	// If successful returns 1. Otherwise returns 0.
	struct inv_item* inventory = player->inventory;
	if ((inventory + index)->amount > 0) {
		// Items effect is based on index. Index represents the item.
		switch (index) {
			// Cases are named after item's names.
			
			// Mikstura HP (M) - increases player hp by 50
			case 0:
				if (player->hp + 50 > player->max_hp) {
					player->hp = player->max_hp;
				}
				else {
					player->hp += 50;
				}
				break;
			// Mikstura HP (S) - increases player hp by 200
			case 1:
				if (player->hp + 200 > player->max_hp) {
					player->hp = player->max_hp;
				}
				else {
					player->hp += 200;
				}
				break;

			// Mikstura HP (2) - fully restores player's hp.
			case 2:
				player->hp = player->max_hp;
				break; 
		}
		(inventory + index)->amount--;
		return 1;
	}
	else {
		return 0;
	}
}

char remove_item(struct inv_item* inventory, int index) {
	// Function removes 1 item from inventory by given index.
	// If successful returns 1. Otherwise returns 0;
	if ((inventory + index)->amount > 0) {
		(inventory + index)->amount--;
		return 1;
	}
	else {
		return 0;
	}
}

int give_item(struct inv_item* inventory, int index, int amount) {
	// Function increases amount of item[index] by amount.
	// Returns number of items that didn't fit in the inventory.

	// Amount of items that can fit in the inventory.
	int max_difference = (inventory + index)->max_amount - (inventory + index)->amount;
	
	// The item is full or there is more items in the inventory than possible for some reason.
	if (max_difference <= 0) {
		return amount;
	}

	// You can fit all of the items.
	if (max_difference >= amount) {
		((inventory+index)->amount) += amount;
		amount = 0;
	}

	// You can fit only limited amount of items.
	else {
		((inventory+index)->amount) += max_difference;
		amount -= max_difference;
	}
	return amount;
}

void clean_item(struct Field* board, struct board_item* item, int k) {
	(board + (item->row) * k + (item->col))->item = 0;
}

char start_encounter(struct Player* player, struct Enemy* enemy) {
	char outcome = -1;
	int picked = 1, action = 0;
	struct Enemy_enc enemy_enc;
	enemy_enc.type = enemy->type;
	setup_enc_enemy(&enemy_enc);
	do {
		do {
			print_player_status(player);
			printf("\n");

			print_enemy_enc_status(&enemy_enc);
			printf("\n");

			print_enc_options(picked);
			list_navigator(&picked, &action, 1, 2);
			system("CLS");
		} while (action == 0);

		switch (action) {
			// Attack
			case 1:
				enemy_enc.hp -= player->attack;
				printf("Player deals %d to enemy.\n", player->attack);
				break;
			case 2:
				// TODO: implement more player stats.
				player->used_defend = 1;
				printf("Player uses defend.\n");
				break;
		}

		if (enemy_enc.hp <= 0) {
			outcome = 1;
			break;
		}

		switch (enemy_enc.type) {
			case 'A':
				goblin_behavior(player, &enemy_enc);
				break;
			case 'B':
				cyclops_behavior(player, &enemy_enc);
				break;
		}

		if (player->hp <= 0) {
			outcome = -1;
			break;
		}

	} while (outcome == -1);
	return outcome;
}

void print_enemy_enc_status(struct Enemy_enc* enemy_enc){
	printf("Enemy status: \n");
	printf("HP: %d/%d, ATK: %d\n", enemy_enc->hp, enemy_enc->hp_max, enemy_enc->attack);
}

void print_enc_options(int picked) {
	printf("Pick your option.\n\n");
	if (picked == 1) {
		printf("->");
	}
	printf("1. Attack\n");

	if (picked == 2) {
		printf("->");
	}
	printf("2. Defend\n");
}
;
void setup_enc_enemy(struct Enemy_enc* enemy_enc) {
	printf("Enemy type: %c", enemy_enc->type);
	switch (enemy_enc->type) {
		case 'A':
			enemy_enc->hp = 100;
			enemy_enc->hp_max = 100;
			enemy_enc->attack = 10;
			printf("You have encountered a goblin!\n\n");
			break;
		case 'B':
			enemy_enc->hp = 200;
			enemy_enc->hp_max = 200;
			enemy_enc->attack = 50;
			printf("You have encountered a cyclops!\n\n");
			break;
	}
}

void goblin_behavior(struct Player* player, struct Enemy_enc* enemy_enc) {
	player->hp -= enemy_enc->attack;
	printf("Goblin attacks and deals %d dmg to player.\n", enemy_enc->attack);
}

void cyclops_behavior(struct Player* player, struct Enemy_enc* enemy_enc) {
	int probs = rand_min_max(0, 4);
	if (probs == 4) {
		player->hp -= enemy_enc->attack;
		printf("Cyclops directly hits the player. Deals %d dmg.\n", enemy_enc->attack);
	}
	else {
		player->hp -= probs;
		printf("Cyclops missed player and hit the ground. Dealt %d dmg.\n", probs);
	}
}

void clean_enemy(struct Field* board, struct Enemy* enemy, int k) {
	(board + (enemy->row) * k + enemy->col)->enemy = 0;
}

// TODO: algorithm to generate a list of random unique intergers from 0 to given N.
void random_unique_numbers(int* num_arr, int size, int n) {
	int i = 0;
	while (i < size) {

	}
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

int rand_min_max(int min, int max) {
	if (min > max) {
		switch_ints(&min, &max);
	}
	return rand() % (abs(max - min) + 1) + min;
}

void switch_ints(int* x, int* y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}


#include<iostream>
#include<iomanip>
#include<stack>
#include<ctime>
#include<cstdlib>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// CONSTANT VARIABLES ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int LINE_SIZE = 120;					//Length of the program's lines displayed
const char TITLE[] = { "Target 9 Game" };	//Title of the program
const int GRID_SIZE = 3;					//The size of the grid

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// GLOBAL VARIABLES ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int grid[GRID_SIZE][GRID_SIZE];				//Grid of game
int target_number = 9;						//The target number

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////// CLASSES //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Class for storing moves
class Move {
public:
	int row, column;

	Move(int, int);
};

//Stores row and column of the grid
Move::Move(int r, int c) {
	row = r;
	column = c;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// FUNCTION HEADERS ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void line();
void header(const char[]);
bool int_check(const int, const int, const int);
bool question(const char[]);
void set_target();
int difficulty_set();
void initialize_grid(const int);
void undo_grid(const int, const int);
void redo_grid(const int, const int);
void print_grid();
void display_grid(const int, const int);
void game_information(const int, const int);
bool win_check();
void move_choice(int&, int&);
void options(stack<Move>&, stack<Move>&);
int options_choice();
void choice_implementation(const int, int&, stack<Move>&, stack<Move>&);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////// MAIN FUNCTION ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
	do {
		int counter = 0, difficulty;

		stack<Move> undoStack;					//Stack for undo
		stack<Move> redoStack;					//Stack for redo

		system("cls");
		header(TITLE);
		set_target();

		difficulty = difficulty_set();
		initialize_grid(difficulty);

		do {
			int choice = 1;

			display_grid(difficulty, counter);

			if (!undoStack.empty() || !redoStack.empty()) {
				options(undoStack, redoStack);
				choice = options_choice();
			}

			choice_implementation(choice, counter, undoStack, redoStack);
		} while (!win_check());

		display_grid(difficulty, counter);

		cout << "You have won in " << counter << (counter == 1 ? " move" : " moves") << endl;
		line();
	} while (question("Do you want to play again?"));

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////// FUNCTIONS /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Print's a line of preconfigured lenght
void line() {
	for (int r = 0; r < LINE_SIZE; r++) {
		cout << "-";
	}
	cout << endl;
}

//Print's a given title
void header(const char str[]) {
	line();
	cout << right << setw((LINE_SIZE / 2) + strlen(str) / 2) << str << endl;
	line();
}

//Checks if an input is an intiger and between given limits
bool int_check(const int min, const int max, const int input) {
	if (!cin.good() || (input < min || input > max)) {
		cin.clear();
		cin.ignore(256, '\n');
		cout << "\aIncorect Value! Give an integer number between " << min << "-" << max << endl << endl;
		return false;
	}
	else {
		return true;
	}
}

//Ask's given question, Return True if yes, False otherwise
bool question(const char str[]) {
	bool check_if;
	char repeat;

	do {
		cout << str << "(Y/N): ";
		cin >> repeat;

		if (repeat != 'y' && repeat != 'Y' && repeat != 'n' && repeat != 'N') {
			cout << "'" << repeat << "'\a is not recognized as a program command." << endl << endl;
			check_if = true;
		}
		else {
			check_if = false;
		}
	} while (check_if);

	return repeat == 'y' || repeat == 'Y' ? true : false;
}

//Asks the user to set a target number
void set_target() {
	int number;

	do {
		cout << "Target number(0-9): ";
		cin >> number;
	} while (!int_check(0, 9, number));

	target_number = number;
}

//Ask's the user to set the difficulty, returns difficulty
int difficulty_set() {
	int number;

	do {
		cout << "Difficulty(1-9): ";
		cin >> number;
	} while (!int_check(1, 9, number));

	return number;
}

//Initializes the grid
void initialize_grid(const int difficulty) {
	srand(time(0));

	//To initialize the grid with the target number
	for (int c = 0; c < GRID_SIZE; c++) {
		for (int r = 0; r < GRID_SIZE; r++) {
			grid[c][r] = target_number;
		}
	}

	//Pick a random row and column to change
	for (int c = 0; c < difficulty; c++) {
		int row = rand() % GRID_SIZE;
		int col = rand() % GRID_SIZE;

		undo_grid(row, col);
	}
}

//Decriments the numbers of same row and column by 1
void undo_grid(const int row, const int column) {
	grid[row][column] = (grid[row][column] + 9) % 10;

	for (int c = 0; c < GRID_SIZE; c++) {
		if (c != column) {
			grid[row][c] = (grid[row][c] + 9) % 10;
		}
	}

	for (int r = 0; r < GRID_SIZE; r++) {
		if (r != row) {
			grid[r][column] = (grid[r][column] + 9) % 10;
		}
	}
}

//Incriments the numbers of same row and column by 1
void redo_grid(const int row, const int column) {
	grid[row][column] = (grid[row][column] + 1) % 10;

	for (int c = 0; c < GRID_SIZE; c++) {
		if (c != column) {
			grid[row][c] = (grid[row][c] + 1) % 10;
		}
	}

	for (int r = 0; r < GRID_SIZE; r++) {
		if (r != row) {
			grid[r][column] = (grid[r][column] + 1) % 10;
		}
	}
}

//Prints the grid
void print_grid() {
	for (int c = 0; c < GRID_SIZE; c++) {
		cout << right << setw((LINE_SIZE / 2) - ((GRID_SIZE)-1)) << " ";

		for (int r = 0; r < GRID_SIZE; r++) {
			cout << left << setw(2) << grid[c][r];
		}

		cout << endl;
	}
}

//Prints the grid and relevant info
void display_grid(const int difficulty, const int counter) {
	system("cls");
	header(TITLE);

	cout << "This is your grid: " << endl;
	print_grid();
	game_information(difficulty, counter);
	line();
}

//Prints difficulty, target number and moves made to terminal
void game_information(const int difficulty, const int counter) {
	cout << "Target: " << target_number << " | Difficulty: " << difficulty << right << setw(93) << "Moves: " << counter << endl;
}

//Checks if all the numbers in the grid are equal to target number
bool win_check() {
	for (int c = 0; c < GRID_SIZE; c++) {
		for (int r = 0; r < GRID_SIZE; r++) {
			if (grid[c][r] != target_number) {
				return false;
			}
		}
	}

	return true;
}

//Tells the user to make a move and returns throu parameters the moves
void move_choice(int& row, int& column) {
	cout << endl << "Enter your move[Row][Column]" << endl;

	do {
		cout << "Row(0" << "-" << GRID_SIZE - 1 << "): ";
		cin >> row;
	} while (!int_check(0, GRID_SIZE - 1, row));

	do {
		cout << "Column(0" << "-" << GRID_SIZE - 1 << "): ";
		cin >> column;
	} while (!int_check(0, GRID_SIZE - 1, column));
}

//Displays the options for the game
void options(stack<Move>& undoStack, stack<Move>& redoStack) {
	cout << "Those are your choices:" << endl;
	cout <<"1. Play" << endl;

	cout << "2. Undo choice";
	if (!undoStack.empty()) {
		cout << " [" << undoStack.top().row << "][" << undoStack.top().column << "]" << endl;
	}
	else {
		cout << " (Not applicable)" << endl;
	}

	cout << "3. Redo choice";
	if (!redoStack.empty()) {
		cout << " [" << redoStack.top().row << "][" << redoStack.top().column << "]" << endl;
	}
	else {
		cout << " (Not applicable)" << endl;
	}
}

//Returns the choice of user
int options_choice() {
	int number;

	do {
		cout << endl << "What would you like to do? ";
		cin >> number;
	} while (!int_check(1, 3, number));

	return number;
}

//Implements the choice of the user
void choice_implementation(const int choice, int& counter, stack<Move>& undoStack, stack<Move>& redoStack) {
	int row, column;

	if (choice == 1) {
		move_choice(row, column);
		redo_grid(row, column);
		counter++;
		undoStack.push(Move(row, column));

		while (!redoStack.empty()) {
			redoStack.pop();
		}
	}
	else if (choice == 2 && !undoStack.empty()) {
		undo_grid(undoStack.top().row, undoStack.top().column);
		redoStack.push(undoStack.top());
		undoStack.pop();
		counter--;
	}
	else if (choice == 3 && !redoStack.empty()) {
		redo_grid(redoStack.top().row, redoStack.top().column);
		undoStack.push(redoStack.top());
		redoStack.pop();
		counter++;
	}
}
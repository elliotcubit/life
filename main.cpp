#include <ncurses.h>
#include <time.h>
#include <stdlib.h>


class Map {
private:
	char ** one;
	char ** two;
	int rows;
	int cols;
	int delay;

public:
	Map();
	~Map();

	bool stop;
	void updateScreen();
	void nextGeneration();
	void loop();
	int getNeighbors(int, int);
};


Map::Map(){
	getmaxyx(stdscr, rows, cols);

	stop = false;
	delay = 250;
	one = new char*[rows];
	two = new char*[rows];
	for (int i=0; i<rows; i++){
		one[i] = new char[cols];
		two[i] = new char[cols];
	}

	// 1/2 change to be alive
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			one[i][j] = rand() % 2 ? ' ' : 'o';
		}
	}
}

void Map::nextGeneration(){
	// logic goes here 
	// the current state is stored in one,
	// we use that state to put the new state in two.

	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){

			int neighbors = getNeighbors(i, j);
			char result;

			switch( neighbors ){
				case 0: case 1:
					two[i][j] = ' '; break;
				case 2:
					two[i][j] = one[i][j]; break;
				case 3:
					two[i][j] = 'o'; break;
				default:
					two[i][j] = ' '; break;

			}
		}
	}



	// afterwards, the "current" state will be in two,
	// and the old will be in one.
	// if we just switch the pointers, don't need to do
	// any copying
	char ** tmp = one;
	one = two;
	two = tmp;
}

int Map::getNeighbors(int x, int y){
	// wrap around

	int up = x-1;
	int down = (x+1)%rows;
	int left = y-1;
	int right = (y+1)%cols;

	if (up < 0)
		up = rows-1;
	if (left < 0)
		left = cols - 1;

	int sum = 0;

	sum += one[up][left] == 'o' ? 1  : 0;
	sum += one[up][y] == 'o' ? 1  : 0;
	sum += one[up][right] == 'o' ? 1  : 0;
	sum += one[x][left] == 'o' ? 1  : 0;
	sum += one[x][right] == 'o' ? 1  : 0;
	sum += one[down][left] == 'o' ? 1  : 0;
	sum += one[down][y] == 'o' ? 1  : 0;
	sum += one[down][right] == 'o' ? 1  : 0;

	return sum;
}

void Map::updateScreen(){
	clear();

	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			addch(one[i][j]);
		}
	}
}

void Map::loop(){
	while (!stop){
		nextGeneration();
		updateScreen();

		char ch = getch();

		if (ch == 'q'){
			break;
		}

		if (ch == '='){
			if (delay > 0) delay -= 50;
		}
	
		if (ch == '-'){
			if (delay < 2000) delay += 50;
		}

		if (ch == 's'){
			nodelay(stdscr, false);

			for ( ;; ){
				char c = getch();
				if (c == 's' || c == 'r'){
					break;
				}
				if (c == 'n'){
					nextGeneration();
					updateScreen();
				}
			}

			nodelay(stdscr, true);
		}

		napms(delay);
	}		
}

void init() {
	srand(time(NULL));
	initscr();
	raw();
	noecho();
	keypad(stdscr, true);
	nodelay(stdscr, true);
	curs_set(0);
}

int main(){
	init();

	Map * m;

	m = new Map();


	m->loop();

	endwin();
}

#include "term.h"

enum ExtraCursesColors {
	COLOR_DARKBLUE = 9,
	COLOR_BRICK,
	COLOR_TEAL,
	COLOR_LIGHTGREY,
	COLOR_DARKGREY
};

static void bind_colors()
{
	// intialize custom colors
	init_color(COLOR_DARKBLUE, 29, 0, 158);
	init_color(COLOR_BRICK, 221, 0, 0);
	init_color(COLOR_TEAL, 0, 234, 255);
	init_color(COLOR_LIGHTGREY, 158, 158, 158);
	init_color(COLOR_DARKGREY, 106, 106, 106);
	// intialize custom pairs (faces)
	init_pair(FACE_MINE, COLOR_WHITE, COLOR_RED);
	init_pair(FACE_EMPTY, COLOR_BLACK, COLOR_BLACK);
	init_pair(FACE_ONE, COLOR_BLUE, COLOR_BLACK);
	init_pair(FACE_TWO, COLOR_GREEN, COLOR_BLACK);
	init_pair(FACE_THREE, COLOR_RED, COLOR_BLACK);
	init_pair(FACE_FOUR, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(FACE_FIVE, COLOR_BRICK, COLOR_BLACK);
	init_pair(FACE_SIX, COLOR_TEAL, COLOR_BLACK);
	init_pair(FACE_SEVEN, COLOR_DARKGREY, COLOR_BLACK);
	init_pair(FACE_EIGHT, COLOR_LIGHTGREY, COLOR_BLACK);
	init_pair(FACE_COVERED, COLOR_WHITE, COLOR_BLACK);
	init_pair(FACE_FLAG, COLOR_RED, COLOR_WHITE);
}

void init_term()
{
	initscr(); // init screen
	cbreak();  // disable line buffering
	keypad(stdscr, TRUE); // enable keypad
	nl(); // enter key sends '\n'
	noecho(); // don't echo typed kety
	curs_set(2); // try to enable block cursor
	start_color(); // enable colors
	bind_colors(); // create custom colors
}

void end_term()
{
	endwin();
}

void add_color_char(int c, int color_pair, int x, int y)
{
	attron(COLOR_PAIR(color_pair));
	mvaddch(y, x, c);
	attroff(COLOR_PAIR(color_pair));
}

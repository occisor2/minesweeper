#pragma once

#include <ncurses.h>

// redefine Enter key to be a newline
#ifdef  KEY_ENTER
#undef KEY_ENTER
#endif

#define KEY_ENTER '\n'

enum TileColor {
	FACE_MINE = 1,
	FACE_EMPTY,
	FACE_ONE,
	FACE_TWO,
	FACE_THREE,
	FACE_FOUR,
	FACE_FIVE,
	FACE_SIX,
	FACE_SEVEN,
	FACE_EIGHT,
	FACE_COVERED,
	FACE_FLAG
};

void init_term();
void end_term();
void add_color_char(int c, int color_pair, int x, int y);

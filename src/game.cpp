#include "game.h"
#include <ncurses.h>
#include <sstream>

enum
{
	MINE_FACE = 1,
	EMPTY_FACE,
	ONE_FACE,
	TWO_FACE,
	THREE_FACE,
	FOUR_FACE,
	FIVE_FACE,
	COVERED_FACE,
	FLAG_FACE
};

Game::Game(Game::Options options)
	: options(options),
	  board(options.width, options.height, options.mines),
	  xpos(0), ypos(0)
{}

void Game::start()
{
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(2);

	start_color();
	init_pair(MINE_FACE, COLOR_WHITE, COLOR_RED);
	init_pair(EMPTY_FACE, COLOR_BLACK, COLOR_BLACK);
	init_pair(ONE_FACE, COLOR_BLUE, COLOR_BLACK);
	init_pair(TWO_FACE, COLOR_GREEN, COLOR_BLACK);
	init_pair(THREE_FACE, COLOR_RED, COLOR_BLACK);
	init_pair(FOUR_FACE, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(FIVE_FACE, COLOR_CYAN, COLOR_BLACK);
	init_pair(FLAG_FACE, COLOR_RED, COLOR_WHITE);

	for (;;) {
		draw();
		if (handleInput())
			break;
	}

	endwin();
}

bool Game::handleInput()
{
	int c = getch();

	switch (c) {
	case 'q':
		return true;
	case KEY_LEFT:
		if (xpos - 1 >= 0) {
			xpos -= 1;
		}
		break;
	case KEY_RIGHT:
		if (xpos + 1 < board.getWidth()) {
			xpos += 1;
		}
		break;
	case KEY_DOWN:
		if (ypos + 1 < board.getHeight()) {
			ypos += 1;
		}
		break;
	case KEY_UP:
		if (ypos - 1 >= 0) {
			ypos -= 1;
		}
		break;
	case 'c':
		// check if the revealed tile was a mine
		if (board.chordTile(xpos, ypos)) {
			if (!displayEndWindow())
				// reset the board
				board = Board(options.width, options.height, options.mines);
			else
				return true;
		}
		break;
	case '\n':
		// check if the revealed tile was a mine
		if (board.revealTile(xpos, ypos)) {
			if (!displayEndWindow())
				// reset the board
				board = Board(options.width, options.height, options.mines);
			else
				return true;
		}
		break;
	case 'f':
		board.flagTile(xpos, ypos);
		break;
	case 'n':
		board = Board(options.width, options.height, options.mines);
		break;
	}

	return false;
}

void Game::draw()
{
	int starty = (LINES - board.getHeight()) / 2;
	int startx = (COLS - board.getWidth() * 2) / 2;

	erase();

	std::stringstream ss;
	ss << "Mines: " << board.getMines() << "; ";
	ss << "Flags: " << board.getFlags() << "; ";
	ss << "Covered: " << board.getCoveredTiles() << "; ";
	ss << "Position: " << xpos << ", " << ypos;
	// display user information
	mvaddstr(0, 0, ss.str().c_str());

	for (int row = 0; row < board.getHeight(); ++row)
	{
		for (int col = 0; col < board.getWidth() * 2; col += 2)
		{
			auto& mask = board.getMask();
			switch (mask[row][col / 2]) {
			case Board::MaskTile::Mine:
				attron(COLOR_PAIR(MINE_FACE));
				mvaddch(starty + row, startx + col, 'M');
				attroff(COLOR_PAIR(MINE_FACE));
				break;
			case Board::MaskTile::Empty:
				attron(COLOR_PAIR(EMPTY_FACE));
				mvaddch(starty + row, startx + col, '*');
				attroff(COLOR_PAIR(EMPTY_FACE));
				break;
			case Board::MaskTile::One:
				attron(COLOR_PAIR(ONE_FACE));
				mvaddch(starty + row, startx + col, '1');
				attroff(COLOR_PAIR(ONE_FACE));
				break;
			case Board::MaskTile::Two:
				attron(COLOR_PAIR(TWO_FACE));
				mvaddch(starty + row, startx + col, '2');
				attroff(COLOR_PAIR(TWO_FACE));
				break;
			case Board::MaskTile::Three:
				attron(COLOR_PAIR(THREE_FACE));
				mvaddch(starty + row, startx + col, '3');
				attroff(COLOR_PAIR(THREE_FACE));
				break;
			case Board::MaskTile::Four:
				attron(COLOR_PAIR(FOUR_FACE));
				mvaddch(starty + row, startx + col, '4');
				attroff(COLOR_PAIR(FOUR_FACE));
				break;
			case Board::MaskTile::Five:
				attron(COLOR_PAIR(FIVE_FACE));
				mvaddch(starty + row, startx + col, '5');
				attroff(COLOR_PAIR(FIVE_FACE));
				break;
			case Board::MaskTile::Six:
				mvaddch(starty + row, startx + col, '6');
				break;
			case Board::MaskTile::Seven:
				mvaddch(starty + row, startx + col, '7');
				break;
			case Board::MaskTile::Eight:
				mvaddch(starty + row, startx + col, '8');
				break;
			case Board::MaskTile::Covered:
				mvaddch(starty + row, startx + col, '#');
				break;
			case Board::MaskTile::Flag:
				attron(COLOR_PAIR(FLAG_FACE));
				mvaddch(starty + row, startx + col, 'F');
				attroff(COLOR_PAIR(FLAG_FACE));
				break;
			}
		}
	}

	int xNewPos = startx + xpos * 2;
	int yNewPos = starty + ypos;

	move(yNewPos, xNewPos);

	refresh();
}

bool Game::displayEndWindow()
{
	// hide cursor during menu
	curs_set(0);
	// display the loosing game state
	draw();
	// prompt user for input
	static constexpr std::string looseMsg = "Game Over";
	int msgStartx = (COLS - looseMsg.size()) / 2;
	int msgStarty = 2;

	mvaddstr(msgStarty, msgStartx, looseMsg.c_str());
	// refresh to show message
	refresh();

	int c = getch();
	// restore cursor before returning
	curs_set(2);
	// return true if player wants to quit
	return 'q' == c;
}

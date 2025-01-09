#include "game.h"
#include "term.h"
#include <sstream>

Game::Game(Game::Options options)
	: options(options),
	  board(options.width, options.height, options.mines),
	  xpos(0), ypos(0)
{}

void Game::start()
{
	init_term();

	for (;;) {
		draw();
		if (handleInput())
			break;
	}

	end_term();
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
	case KEY_ENTER:
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
				add_color_char('M', FACE_MINE, startx + col, starty + row);
				break;
			case Board::MaskTile::Empty:
				add_color_char('*', FACE_EMPTY, startx + col, starty + row);
				break;
			case Board::MaskTile::One:
				add_color_char('1', FACE_ONE, startx + col, starty + row);
				break;
			case Board::MaskTile::Two:
				add_color_char('2', FACE_TWO, startx + col, starty + row);
				break;
			case Board::MaskTile::Three:
				add_color_char('3', FACE_THREE, startx + col, starty + row);
				break;
			case Board::MaskTile::Four:
				add_color_char('4', FACE_FOUR, startx + col, starty + row);
				break;
			case Board::MaskTile::Five:
				add_color_char('5', FACE_FIVE, startx + col, starty + row);
				break;
			case Board::MaskTile::Six:
				add_color_char('6', FACE_SIX, startx + col, starty + row);
				break;
			case Board::MaskTile::Seven:
				add_color_char('7', FACE_SEVEN, startx + col, starty + row);
				break;
			case Board::MaskTile::Eight:
				add_color_char('8', FACE_EIGHT, startx + col, starty + row);
				break;
			case Board::MaskTile::Covered:
				add_color_char('#', FACE_COVERED, startx + col, starty + row);
				break;
			case Board::MaskTile::Flag:
				add_color_char('F', FACE_FLAG, startx + col, starty + row);
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

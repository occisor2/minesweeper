#include "board.h"
#include <cassert>
#include <random>
#include <chrono>

/**
 * Helpfull shorcut macro for asserting whether an x and y point are on the
 * board.
 */
#ifndef NDEBUG
#define ASSERT_POSITION(x, y)									\
	(assert(((void) "position out of bounds",					\
			 x >= 0 && x < width &&	y >= 0 && y < height)))
#else // NDEBUG
#define ASSERT_POSITION(x, y) ((void))
#endif // NDEBUG

Board::Board(int width, int height, int mines)
	: width(width), height(height), mines(mines), flags(0), coveredTiles(width * height),
	  board(height, std::vector<int>(width, 0)),
	  mask(height, std::vector<MaskTile>(width, MaskTile::Covered))
{
	std::random_device rd;
	// seed using time and random device
	std::seed_seq seed{rd(), static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
	randomGen = std::mt19937(seed);
}


int Board::getWidth() const
{
	return width;
}

int Board::getHeight() const
{
	return height;
}

int Board::getMines() const
{
	return mines;
}

int Board::getFlags() const
{
	return flags;
}

int Board::getCoveredTiles() const
{
	return coveredTiles;
}

const std::vector<std::vector<Board::MaskTile>>& Board::getMask() const
{
	return mask;
}

bool Board::revealTile(int x, int y)
{
	ASSERT_POSITION(x, y);
	// If no tiles have been revealed yet, generate mines around this first
	// starting move.
	if (width * height == coveredTiles)
		generateMines(x, y);

	auto boardSquare = board[y][x];
	// only try to reveal if tile hasn't been revealed yet
	if (MaskTile::Covered == mask[y][x]) {
		if (0 == boardSquare) {
			floodFill(x, y);
		} else {
			mask[y][x] = boardToMaskTile(boardSquare);
			--coveredTiles;
		}
		// return true if the player loses
		return -1 == boardSquare;
	}

	return -1 == boardSquare;
}

void Board::flagTile(int x, int y)
{
	ASSERT_POSITION(x, y);
	// only flag tiles that are covered
	if (MaskTile::Covered == mask[y][x]) {
		mask[y][x] = MaskTile::Flag;
		++flags;
	}
	// if tile is already flagged, unflag it
	else if (MaskTile::Flag == mask[y][x]) {
		mask[y][x] = MaskTile::Covered;
		--flags;
	}
}

bool Board::chordTile(int x, int y)
{
	ASSERT_POSITION(x, y);

	auto tileValue = board[y][x];
	// check that the tile is uncovered and and numbered
	if (mask[y][x] == MaskTile::Covered || mask[y][x] == MaskTile::Flag || tileValue <= 0)
		return false;
	// compare value to number of flags
	auto count = countAdjacentFlags(x, y);
	if (!(count >= tileValue))
		return false;
	// check and reveal each adjacent tiles
	for (int di = -1; di <= 1; ++di) {
		for (int dj = -1; dj <= 1; ++dj) {
			int ni = y + di; // neighbor row index
			int nj = x + dj; // neighbor column index
			// check if the neighbor index is within bounds
			if (!(0 == di && 0 == dj) && ni >= 0 && ni < height && nj >= 0 && nj < width) {
				// ignore correctly flags
				if (!(MaskTile::Flag == mask[ni][nj] && -1 == board[ni][nj]))
					// check for incorrectly placed flags
					if (revealTile(nj, ni))
						return true;
			}
		}
	}

	return false;
}

bool Board::inStartingArea(int startx, int starty, int x, int y) const
{
	if (startx == x && starty == y)
		return true;

	if (x >= startx - 1 && x <= startx + 1 && y >= starty - 1 && y <= starty + 1)
		return true;

	return false;
}

/**
 * Place mines randomly on the board, but leave empty space around the players
 * first click.
 */
void Board::generateMines(int startx, int starty)
{
	std::uniform_int_distribution<> xdist(0, width - 1);
	std::uniform_int_distribution<> ydist(0, height - 1);

	for (int i = 0; i < mines; ++i) {
		int x = xdist(randomGen);
		int y = ydist(randomGen);

		if (inStartingArea(startx, starty, x, y)) {
			--i;
			continue;
		}

		board[y][x] = -1;
	}

	initAdjacentMineCounts();
}

/**
 * Intializes mine counts of non-mine board tiles.
 *
 * Check all around squares for mines and add number of mines to
 * square.
 *
 * M # M
 * # 5 M
 * # M M
 */
void Board::initAdjacentMineCounts()
{
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			// skip square if it's a mine
			if (-1 == board[row][col])
				continue;

			for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    int ni = row + di; // neighbor row index
                    int nj = col + dj; // neighbor column index
                    // check if the neighbor index is within bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                        if (-1 == board[ni][nj])
                            board[row][col] += 1;
                }
            }
		}
	}
}

Board::MaskTile Board::boardToMaskTile(int boardSquare) const
{
	assert(((void) "board piece not a valid mask tile", boardSquare >= -1 && boardSquare <= 10));
	return static_cast<MaskTile>(boardSquare);
}

/**
 * Uncover all surrounding empty and numbered tiles.
 *
 * Implements an inefficient flood fill algorithm, so look into optimizing.
 *
 * 1 1 1
 * 1 * 1
 * 1 1 1
 */
void Board::floodFill(int x, int y)
{
	ASSERT_POSITION(x, y);
	// only check covered or flagged tiles
	if (MaskTile::Covered != mask[y][x] && MaskTile::Flag != mask[y][x])
		return;

	auto boardSquare = board[y][x];

	if (MaskTile::Flag == mask[y][x])
		--flags;
	mask[y][x] = boardToMaskTile(boardSquare);
	--coveredTiles;
	// end recursion if the current tile is not empty
	if (boardSquare != 0)
		return;
	// fill adjacent tiles
	for (int di = -1; di <= 1; ++di) {
		for (int dj = -1; dj <= 1; ++dj) {
			int ni = y + di; // neighbor row index
			int nj = x + dj; // neighbor column index
			// check if the neighbor index is within bounds
			if (ni >= 0 && ni < height && nj >= 0 && nj < width)
				floodFill(nj, ni);
		}
	}
}

int Board::countAdjacentFlags(int x, int y)
{
	ASSERT_POSITION(x, y);

	int flagCount = 0;
	for (int di = -1; di <= 1; ++di) {
		for (int dj = -1; dj <= 1; ++dj) {
			int ni = y + di; // neighbor row index
			int nj = x + dj; // neighbor column index
			// check if the neighbor index is within bounds
			if (!(0 == di && 0 == dj) && ni >= 0 && ni < height && nj >= 0 && nj < width)
				if (MaskTile::Flag == mask[ni][nj])
					++flagCount;
		}
	}

	return flagCount;
}

#pragma once

#include <random>
#include <vector>

class Board
{
public:
	enum class MaskTile
	{
		Mine = -1,
		Empty,
		One,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Covered,
		Flag
	};

	explicit Board(int width = 9, int height = 9, int mines = 10);

	int getWidth() const;
	int getHeight() const;
	int getMines() const;
	int getFlags() const;
	int getCoveredTiles() const;
	const std::vector<std::vector<MaskTile>>& getMask() const;
	/**
	 * Reveal selected tiles.
	 *
	 * The first tile revealed is guaranteed to not touch any mines.
	 *
	 * If the tile is a mine, return true, which signals the end of the game. If the
	 * tile is a zero tile, reveal all adjacent non-mine tiles. Number tiles are
	 * revealed normally.
	 *
	 * When the flood fill algorithm is run to reveal adjacent tiles, flags are
	 * removed from incorrectly flagged tiles.
	 */
	bool revealTile(int x, int y);
	/**
	 * Marks a tile as flagged.
	 *
	 * If a tile was already flaggged, remove the flag.
	 */
	void flagTile(int x, int y);
	/**
	 * Reveals adjacent tiles if the selected tile's number equals the number of
	 * flags surrounding it.
	 *
	 * If the flags are incorrectly placed, true is returned and the mines are
	 * revealed.
	 */
	bool chordTile(int x, int y);

private:
	int width{};
	int height{};
	int mines{};
	int flags{};
	int coveredTiles{};
	/**
	 * -1 mine
	 * 0 empty tile
	 * 1-9 mines are touching adjacent tiles
	 */
	std::vector<std::vector<int>> board;
	std::vector<std::vector<MaskTile>> mask;
	// maybe move into generateMines method since not needed class wide
	std::mt19937 randomGen;

	bool inStartingArea(int startx, int starty, int x, int y) const;
	void generateMines(int startx, int starty);
	void initAdjacentMineCounts();
	MaskTile boardToMaskTile(int boardSquare) const;
	void floodFill(int x, int y);
	int countAdjacentFlags(int x, int y);
};

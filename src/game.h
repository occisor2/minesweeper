#pragma once

#include "board.h"

class Game
{
public:
	struct Options
	{
		int width = 0;
		int height = 0;
		int mines = 0;
	};

	explicit Game(Options options);

	void start();

private:
	Options options;
	Board board;
	int xpos{};
	int ypos{};

	void draw();
	bool handleInput();
	bool displayEndWindow();
};

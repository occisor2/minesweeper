#include <cstdlib>
#include "game.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	Game::Options opt(30, 16, 150);
	Game game(opt);
	game.start();

	return EXIT_SUCCESS;
}

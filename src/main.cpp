#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include "game.h"

static void print_help_message()
{
	std::cout << "usage: msweep [options]" << std::endl;
	std::cout << "  options:" << std::endl;
	std::cout << "    -w <width>" << std::endl;
	std::cout << "    -h <height>" << std::endl;
	std::cout << "    -m <number of mines>" << std::endl;
}

int main(int argc, char* argv[])
{
	int opt{};
	Game::Options gameOpts(30, 16, 99);

	try {
		while ((opt = getopt(argc, argv, "w:h:m:")) != -1) {
			switch (opt) {
			case 'w': {
				int width{std::stoi(optarg)};
				gameOpts.width = width;
				break;
			}
			case 'h': {
				int height{std::stoi(optarg)};
				gameOpts.height = height;
				break;
			}
			case 'm': {
				int mines{std::stoi(optarg)};
				gameOpts.mines = mines;
				break;
			}
			default:
				print_help_message();
				return EXIT_FAILURE;
			}
		}
	} catch (const std::invalid_argument& e) {
		std::cerr << "error: argument is not a valid number" << std::endl;
		return EXIT_FAILURE;
	} catch (const std::out_of_range& e) {
		std::cerr << "error: argument value is too large" << std::endl;
		return EXIT_FAILURE;
	}

	Game game(gameOpts);
	game.start();

	return EXIT_SUCCESS;
}

#include <fstream>
#include "functions.h"

int main()
{
	int board[9][9] = { 0 };

	std::string board_name;
	do
	{
		do
		{
			std::cout << "Please enter board to import (with .txt) or quit to exit: ";
			std::cin >> board_name;


		} while (!import_board(board, board_name) && board_name != "quit");

		if (board_name != "quit")
		{
			std::cout << board << "\n\n";
			int iterations = 0;
			bool solved = solve_board(board, iterations);

			if (solved)
			{
				std::cout << "Number of iterations: " << iterations << "\n\n";
				std::cout << board;
				std::cout << std::endl;
			}
			else
			{
				std::cout << "Board not solved.\n\n";
			}
		}
	} while (board_name != "quit");
	return 0;
}
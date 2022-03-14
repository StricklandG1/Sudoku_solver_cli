#include "functions.h"
#include <fstream>
#include <stack>

#pragma region populate board
/**
 * Import board from text file
 *
 * Allows user to import a 9x9 board from a text file
 * using fstream going top to bottom, left to right
 *
 * @param board The 9x9 grid to contain the imported information
 * @param f_name The name of the file to be imported
 * @return whether board was imported successfully
 *
 */
bool import_board(int board[][9], std::string f_name)
{
	std::ifstream in_file;
	in_file.open(f_name);

	bool result = !in_file.fail();
	if (result)
	{
		for (int i = 0; i < 9; ++i)
		{
			for (int j = 0; j < 9; ++j)
			{
				in_file >> board[i][j];
			}
		}
	}
	return result;
}
#pragma endregion

/**
 * Get first available square
 *
 * Search for the first empty square to begin solving
 *
 * @param board The 9x9 board to be solved
 * @param start_y the starting row of the first empty square
 * @param start_x the starting column of the first empty square
 *
 */
void get_first_move(int board[][9], int& start_y, int& start_x)
{
	bool found = false;
	int i = 0;
	while (!found && i < 9)
	{
		int j = 0;
		while (!found && j < 9)
		{
			if (board[i][j] == 0)
			{
				found = true;
				start_y = i;
				start_x = j;
			}
			++j;
		}
		++i;
	}
}

/**
 * Solve the board
 *
 * Use CSP and backtracking algorithm to solve 9x9 sudoku board
 * Calls check_row, check_col, check_local_grid
 *
 * @param board The 9x9 board to be solved
 * @return whether the board was solved succesfully or not
 *
 */
bool solve_board(int board[][9], int& iterations)
{
	int start_y, start_x;
	get_first_move(board, start_y, start_x);

	int y = start_y;
	int x = start_x;

	std::stack<std::pair<int, std::pair<int, int>>> moves;

	bool solved = false;

	int num = 1;
	iterations = 0;
	while (!solved && x < 9 && y < 9)
	{
		//if (iterations == 3930)
		//{
		//	std::cout << "breakpoint\n\n";
		//}
		bool valid = check_num(num, y, x, board);
		if (valid)
		{
			moves.push({num, { y, x } });
			board[y][x] = num;

			while (board[y][x] != 0)
			{
				y += (x == 8) ? 1 : 0;
				x = (x + 1) % 9;
			}
			num = 1;
		}
		else
		{
			if (num >= 9)
			{
				while (moves.top().first == 9)
				{
					board[moves.top().second.first][moves.top().second.second] = 0;
					moves.pop();
				}
				y = moves.top().second.first;
				x = moves.top().second.second;
				moves.pop();
				num = board[y][x] + 1;
				board[y][x] = 0;
			}
			else
			{
				++num;
			}
		}

		solved = check_solved_board(board);
		++iterations;
		
	}
	return solved;
}


#pragma region number checks
/**
 * Check one number in the board
 *
 * Check whether or not a number is valid by checking
 * the row, column, and grid it will be placed in. If
 * it is, then return true and add it to the board
 *
 * @param num The number to be added
 * @param row_num The row the number will be checked in
 * @param row_col The column the number will be checked in
 * @param board The game board
 * @return true if the constraint is satisfied and number can be place, false otherwise
 */
bool check_num(int num, int row_num, int col_num, int board[][9])
{
	if (num > 9)
	{
		return false;
	}

	bool row_valid = check_row(num, board[row_num]);
	bool col_valid = check_col(num, col_num, board);
	bool grid_valid = check_local_grid(num, row_num / 3, col_num / 3, board);

	return (row_valid && col_valid && grid_valid);
}

/**
 * Checks a row in the grid
 *
 * Checks if a given number is valid, if a row is complete, 
 * or if there are no current valid numbers
 *
 * @param num The number to check
 * @param board The row to check
 * @return true if number is valid in row, false otherwise
 *
 */
bool check_row(int num, int board[])
{
	bool result;
	int i = 0;
	for (int i = 0; i < 9; ++i)
	{
		if (num == board[i])
		{
			return false;
		}
	}
	return true;
}

/**
* Checks a column in the grid
 *
 * Checks if a given number is valid, if a column is complete,
 * or if there are no current valid numbers
 *
 * @param num The number to check
 * @param col_num The column number to check
 * @param board The game board
 * @return true if number is valid in column, false otherwise
 *
 *
*/
bool check_col(int num, int col_num, int board[][9])
{
	for (int i = 0; i < 9; ++i)
	{
		if (num == (board[i][col_num]))
		{
			return false;
		}
	}
	return true;
}

/**
 * Check the local 3x3 grid
 *
 * Checks if a given number is valide, if a grid is complete,
 * or if there are no current valid numbers
 *
 * @param The number to check
 * @param grid_row The 'row' of the 3x3 grid
 * @param grid_col The 'column' of the 3x3 grid
 * @param board The game board
 * @return true if number is valid in grid, false otherwise
*/
bool check_local_grid(int num, int grid_row, int grid_col, int board[][9])
{
	int row_start = grid_row * 3;
	int col_start = grid_col * 3;

	for (int i = row_start; i < row_start + 3; ++i)
	{
		for (int j = col_start; j < col_start + 3; ++j)
		{
			if (num == board[i][j])
			{
				return false;
			}
		}
	}
	return true;
}
#pragma endregion
#pragma region solved checks
/*
 * Check if board is solved
 *
 * Cheack each row, column, and grid to determine whether board
 * is solved
 *
 * @param board the board to be solved
 * @return true if board is solved, false otherwise
 */
bool check_solved_board(int board[][9])
{
	bool solved = true;
	
	int i = 0;
	while (solved && i < 9)
	{
		solved = check_solved_row(board[i]);
		solved = check_solved_column(i, board);
		++i;
	}

	i = 0;
	int j = 0;
	while (solved && i < 3)
	{
		j = 0;
		while (solved && j < 3)
		{
			solved = check_solved_grid(i, j, board);
			++j;
		}
		++i;
	}

	return solved;
}

/*
 * Check if row is solved
 *
 * Checks a row to determine if it is solved
 * 
 *
 * @param board the row to check
 * @return true if row is solved, false otherwise
 */
bool check_solved_row(int board[])
{
	int nums[10] = { 0 };
	for (int i = 0; i < 9; ++i)
	{
		++nums[board[i]];
	}

	for (int i = 1; i < 10; ++i)
	{
		if (nums[i] != 1)
		{
			return false;
		}
	}

	return true;
}

/*
 * Check if column is solved
 *
 * Check a column to determine if it is solved
 * 
 *
 * @param col_num the board to be solved
 * @param board the board to be solved
 * @return true if column is solved, false otherwise
 */
bool check_solved_column(int col_num, int board[][9])
{
	int nums[10] = { 0 };
	for (int i = 0; i < 9; ++i)
	{
		++nums[board[i][col_num]];
	}

	for (int i = 1; i < 10; ++i)
	{
		if (nums[i] != 1)
		{
			return false;
		}
	}

	return true;
}

/*
 * Check if grid is solved
 *
 * Checks a 3x3 grid to determine if it is solved
 * Splits the board into a 3x3 grouping of 3x3 grids using basic arithmatic

 * @param grid_row the simulated row the grid is in
 * @param grid_col the simulated col the grid is in
 * @param board the board to be solved
 * @return true if grid is solved, false otherwise
 */
bool check_solved_grid(int grid_row, int grid_col, int board[][9])
{
	int row_start = grid_row * 3;
	int col_start = grid_row * 3;
	int nums[10] = { 0 };

	for (int i = row_start; i < row_start + 3; ++i)
	{
		for (int j = col_start; j < col_start + 3; ++j)
		{
			++nums[board[i][j]];
		}
	}

	bool valid = true;
	int i = 1;
	while (valid && i < 10)
	{
		valid = (nums[i] == 1);
		++i;
	}
	
	return valid;
}
#pragma endregion

// Overloaded ostream operator to print board easily
std::ostream& operator<<(std::ostream& out, int board[][9])
{
	for (int i = 0; i < 9; ++i)
	{
		out << "| ";
		for (int j = 0; j < 9; ++j)
		{
			out << board[i][j] << " | ";
		}
		out << std::endl;
	}
	return out;
}
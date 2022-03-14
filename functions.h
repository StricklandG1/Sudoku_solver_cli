#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <iostream>
#include <string>

// Populate board
bool import_board(int board[][9], std::string f_name);
void get_first_move(int board[][9], int& start_y, int& start_x);

bool solve_board(int board[][9], int& iterations);
bool check_num(int num, int row_num, int col_num, int board[][9]);
bool check_row(int num, int board[]);
bool check_col(int num, int col_num, int board[][9]);
bool check_local_grid(int num, int grid_row, int grid_col, int board[][9]);

bool check_solved_board(int board[][9]);
bool check_solved_row(int board[]);
bool check_solved_column(int col_num, int board[][9]);
bool check_solved_grid(int grid_row, int grid_col, int board[][9]);



// Print the board
std::ostream& operator<<(std::ostream& out, int board[][9]);
#endif
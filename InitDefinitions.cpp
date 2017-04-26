#include "Definitions.h"
#include "Functions.h"

BoardStates::BoardStates() :shah(EMPTY){};

Board::Board(){}

Board::Board(vector<int8> board, vector<int8> positions, vector<int8> colors, BoardStates state) : board(board), positions(positions), colors(colors), states(state){}

Board base_board;
vector<int8> *moves = new vector<int8>[10];
vector<Special*> g_specials;
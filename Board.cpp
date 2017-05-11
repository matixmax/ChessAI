
#include "Board.h"
#include "Definitions.h"

using namespace std;

Board::Board() {}

Board::Board(vector<int8> board, vector<int8> positions, vector<int8> colors, BoardStates state)
    : board(board), positions(positions), colors(colors), states(state) {}

Board Board::createEmptyBoard()
{
    return Board(vector<int8>(BOARD_SIZE, E), 
        vector<int8>(NUMBER_OF_POSITIONS, E), 
        vector<int8>(BOARD_SIZE, DESTROYED));
}

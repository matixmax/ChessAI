
#include "Board.h"
#include "Definitions.h"

using namespace std;

Board::Board() {}

Board::Board(vector<int> board, vector<int> positions, vector<int> colors, BoardStates state)
    : board(board), positions(positions), colors(colors), states(state) {}

Board Board::createEmptyBoard()
{
    return Board(vector<int>(BOARD_SIZE, E), 
        vector<int>(NUMBER_OF_POSITIONS, DESTROYED), 
        vector<int>(BOARD_SIZE, E));
}

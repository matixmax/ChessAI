#include "Definitions.h"
#include "Board.h"
#include "PreparatoryFunctions.h"
#include "Generator.h"
#include "Valuator.h"
#include "EngineFunctions.h"
#include "SpecialsManager.h"
#include "Debug.h"
#include "Globals.h"

using namespace std;

void initWelcome()
{
	cout << "Welocme player" << endl;
	cout << "To show settings write -1 -1 as your move" << endl;
}

Board makeUserMove(Board last_board)
{
	int curr_pos, next_pos;
	do {
		cin >> curr_pos >> next_pos;
		if (SpecialsManager::I().isSpecialCode(make_pair(curr_pos, next_pos))) {
			SpecialsManager::I().getFormCode(make_pair(curr_pos, next_pos))->update();
			cin >> curr_pos >> next_pos;
		}
	} while (!userMove(curr_pos, next_pos, last_board, BLACK));
	return last_board;
}

void onStartGame()
{
	setInitialBoard();
	setMoves();
	initWelcome();
}

int main() {
	onStartGame();
	Board last_board = g_baseBoard;
	printBoard(last_board);
	while (true) {
		last_board = NormalAlfaBeta(last_board, WHITE, 4);
		if (last_board.states.shah == END) {
			break;
		}
		printBoard(last_board);
		last_board = makeUserMove(last_board);
		printBoard(last_board);
	}
	system("pause");
}
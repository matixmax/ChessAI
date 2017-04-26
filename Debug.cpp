#include "Debug.h"
#include "Definitions.h"
#include "Generator.h"
#include "ValuationFunctions.h"
#include "EngineFunctions.h"
#include "Globals.h"
#include "Board.h"

using namespace std;

vector<Board> Debug::GenerateMoves(const Board &position, int8 color) {
	vector<Board> available_positions;
	Generator generator(position);
	while (generator.HasNext(color))
		available_positions.push_back(generator.GetNextPosition(color));
	return available_positions;
}

void Debug::printValues(const Board &test_board, int8 color) {
	int phase = checkGamePhase(test_board);
	cout << "phase " << phase << endl;
	cout << "material valuation " << materialValuation(test_board, color) << endl;
	if (phase == MATTING)
		cout << "matting pos value " << mattingPositionalValue(test_board, color) << endl;
	else {
		cout << "pawn pos value " << pawnsPositionalValue(test_board, color, phase) << endl;
		cout << "knights pos value " << knightsPositionalValue(test_board, color) << endl;
		cout << "bishops pos value " << bishopsPositionalValue(test_board, color, phase) << endl;
		cout << "rooks pos value " << rooksPositionalValue(test_board, color, phase) << endl;
		cout << "queens pos value " << queenPositionalValue(test_board, color, phase) << endl;
		cout << "king pos value " << kingPositionalValue(test_board, color, phase) << endl;
	}
}

void Debug::posValuationTest() {
	setInitialBoard();
	setMoves();
	Board test_board = g_baseBoard;
	int curr_pos, next_pos;
	while (true) {
		while (true) {
			cin >> curr_pos >> next_pos;
			if (curr_pos == -1)break;
			userMove(curr_pos, next_pos, test_board, BLACK, true);
			printBoard(test_board);
		}
		cout << "WHITE" << endl;
		printValues(test_board, WHITE);
		cout << "BLACK:" << endl;
		printValues(test_board, BLACK);
	}
}

void Debug::boardStatesAndAttackPosTest() {
	setInitialBoard();
	setMoves();
	Board test_board = g_baseBoard;
	int curr_pos, next_pos;
	while (true) {
		while (true) {
			cin >> curr_pos >> next_pos;
			if (curr_pos == -1)break;
			userMove(curr_pos, next_pos, test_board, BLACK);
			printBoard(test_board);
		}
		cin >> next_pos;
		cout << "Possible attack" << endl;
		vector<Board> attack_boards = Generator::GetPosAttackMove(test_board, next_pos, WHITE);
		for (Board position : attack_boards) {
			printBoard(position);
			if (position.states.shah != EMPTY)cout << "SHAH" << endl;
		}
	}
}

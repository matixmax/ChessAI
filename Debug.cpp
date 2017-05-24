
#include "Debug.h"
#include "Definitions.h"
#include "Generator.h"
#include "Valuator.h"
#include "Engine.h"
#include "Globals.h"
#include "Board.h"
#include "Initializator.h"
#include "PawnsValuator.h"
#include "RocksValuator.h"
#include "QueensValuator.h"
#include "KingsValuator.h"

using namespace std;
using namespace Valuation;

vector<Board> Debug::GenerateMoves(const Board &position, int color) {
    vector<Board> availablePositions;
    Generator generator(position);
    while (generator.HasNext(color))
        availablePositions.push_back(generator.GetNextPosition(color));
    return availablePositions;
}

void Debug::printValues(const Board &test_board, int color) {
    GameState phase = Valuator::i().getGamePhase(test_board);
    cout << "phase " << phase << endl;
    cout << "material valuation " << Valuator::i().materialValuation(test_board, color) << endl;
    if (phase == MATTING)
        cout << "matting pos value " << Valuator::i().mattingPositionalValue(test_board, color) << endl;
    else {
        cout << "pawn pos value " << PawnsValuator::getPositionalValue(test_board, color, phase) << endl;
        cout << "knights pos value " << Valuator::i().knightsPositionalValue(test_board, color) << endl;
        cout << "bishops pos value " << Valuator::i().bishopsPositionalValue(test_board, color, phase) << endl;
        cout << "rooks pos value " << RocksValuator::getPositionalValue(test_board, color, phase) << endl;
        cout << "queens pos value " << QueenValuator::getPositionalValue(test_board, color, phase) << endl;
        cout << "king pos value " << KingsValuator::getPositionalValue(test_board, color, phase) << endl;
    }
}

void Debug::posValuationTest() {
    Initializator::init();
    Board test_board = g_baseBoard;
    int curr_pos, next_pos;
    while (true) {
        while (true) {
            cin >> curr_pos >> next_pos;
            if (curr_pos == -1)break;
            Engine::userMove(curr_pos, next_pos, test_board, BLACK, true);
            GuiModule::printBoard(test_board, cout);
        }
        cout << "WHITE" << endl;
        printValues(test_board, WHITE);
        cout << "BLACK:" << endl;
        printValues(test_board, BLACK);
    }
}

void Debug::boardStatesAndAttackPosTest() {
    Initializator::init();
    Board test_board = g_baseBoard;
    int curr_pos, next_pos;
    while (true) {
        while (true) {
            cin >> curr_pos >> next_pos;
            if (curr_pos == -1)break;
            Engine::userMove(curr_pos, next_pos, test_board, BLACK);
            GuiModule::printBoard(test_board, cout);
        }
        cin >> next_pos;
        cout << "Possible attack" << endl;
        vector<Board> attack_boards = Generator::GetOnPositionAttackMove(test_board, next_pos, WHITE);
        for (Board position : attack_boards) {
            GuiModule::printBoard(position, cout);
            if (position.states.shah != EMPTY)cout << "SHAH" << endl;
        }
    }
}

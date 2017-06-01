
#include "Statistics.h"
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
#include "KnightsValuator.h"
#include "BishopsValuator.h"

using namespace std;
using namespace Valuation;

vector<Board> Statistics::GenerateMoves(const Board &position, int color) {
    vector<Board> availablePositions;
    Generator generator(position);
    while (generator.HasNext(color))
        availablePositions.push_back(generator.GetNextPosition(color));
    return availablePositions;
}

void Statistics::printValues(const Board &test_board, int color, ostream &output) {
    GameState phase = Valuator::i().getGamePhase(test_board);
    output << "color " << ((color == WHITE) ? "WHITE" : "BLACK") << endl;
    output << "phase " << ((phase == DEBUT)?"DEBUT":(phase == MID_GAME)?"MID_GAME":(phase == EARLY_ENDING)?"EARLY_ENDING":(phase == ENDING)?"ENDING":"MATTING") << endl;
    auto material = Valuator::i().countMaterial(test_board);
    output << "material WHITE " << material.first << endl;
    output << "material BLACK " << material.second << endl;
    output << "material valuation " << Valuator::i().materialValuation(test_board, color) << endl;
    if (phase == MATTING)
        output << "matting pos value " << Valuator::i().mattingPositionalValue(test_board, color) << endl;
    else {
        output << "pawn pos value " << PawnsValuator::getPositionalValue(test_board, color, phase) << endl;
        output << "knights pos value " << KnightsValuator::getPositionalValue(test_board, color) << endl;
        output << "bishops pos value " << BishopsValuator::getPositionalValue(test_board, color, phase) << endl;
        output << "rooks pos value " << RocksValuator::getPositionalValue(test_board, color, phase) << endl;
        output << "queens pos value " << QueenValuator::getPositionalValue(test_board, color, phase) << endl;
        output << "king pos value " << KingsValuator::getPositionalValue(test_board, color, phase) << endl;
    }
}

void Statistics::printValues(const Board &test_board, int color) {
    return printValues(test_board, color, cout);
}

void Statistics::posValuationTest() {
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

void Statistics::boardStatesAndAttackPosTest() {
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

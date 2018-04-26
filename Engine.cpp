
#include <algorithm>
#include "Engine.h"
#include "Definitions.h"
#include "Generator.h"
#include "Valuator.h"
#include "Board.h"
#include "Globals.h"
#include "FigInfo.h"
#include "PawnsValuator.h"
#include "RocksValuator.h"
#include "PawnsPromotion.h"
#include "QueensValuator.h"
#include "KingsValuator.h"
#include "KnightsValuator.h"
#include "BishopsValuator.h"

#ifdef DEBUG
#include "GuiModule.h"
#include "Statistics.h"
#endif
#include "BoardRememberer.h"

using namespace std;
using namespace Valuation;

bool Engine::checkFiguresMovement(int curr_pos, int next_pos, const Board &chessboard){
    int figure = chessboard.board[curr_pos];
    int distance = 1;
    if (isCastlingMove(curr_pos, next_pos, chessboard)) {
        return true;
    }
    for (unsigned int move_id = 0; move_id < g_figMoves[figure].size() && distance < 8; move_id++){
        if (g_figMoves[figure][move_id] == RANGED_FAR){
            distance++;
            move_id = 0;
        }
        int previus_pos = curr_pos + (distance - 1) * g_figMoves[figure][move_id];
        int pos = curr_pos + distance * g_figMoves[figure][move_id];
        if (pos< 0 || pos >= BOARD_SIZE || abs(pos % 8 - previus_pos % 8) > 2 )continue;
        if (pos == next_pos){
            while (pos != curr_pos){
                pos = pos - g_figMoves[figure][move_id];
                if (pos != curr_pos && chessboard.board[pos] != EMPTY)
                    return false;
            }
            return true;
        }
    }
    if (figure == Pc || figure == Pb){
        if (g_figMoves[figure][0] * 2 + curr_pos == next_pos)
            return true;
        if (chessboard.colors[next_pos] == FigInfo::not(chessboard.colors[curr_pos]) 
                && g_figMoves[figure + 2][0] + curr_pos == next_pos)
            return true;
        if (chessboard.colors[next_pos] == FigInfo::not(chessboard.colors[curr_pos]) 
                && g_figMoves[figure + 2][1] + curr_pos == next_pos)
            return true;
    }
    return false;
}

bool Engine::checkShah(const Board &chessboard, int color){
    auto possibleAttacks = Generator::GetOnPositionAttackMove(chessboard,
                                chessboard.positions[FigInfo::getPosIndex(K, color)],
                                FigInfo::not(color),
                                true);
    if ( possibleAttacks.size() == 0 )
        return false;
    return true;
}

bool Engine::userMove(int curr_pos, int next_pos, Board &chessboard, int color, bool trick_mode){
    int figure = 0;
    if (curr_pos > 63 || curr_pos < 0 || next_pos > 63 || next_pos < 0 || chessboard.board[curr_pos] == EMPTY){
        cout << "bad position " << endl;
        return false;
    }
    while (chessboard.positions[figure] != curr_pos){
        figure++;
        if (figure >= NUMBER_OF_POSITIONS){
            cout << "bad figure" << endl;
            return false;
        }
    }
    if (!trick_mode && !checkFiguresMovement(curr_pos, next_pos, chessboard)){
        cout << "bad movement" << endl;
        return false;
    }
    if (chessboard.board[next_pos] != EMPTY && chessboard.colors[next_pos] == chessboard.colors[curr_pos]) {
        cout << "can't attack myself" << endl;
        return false;
    }
    Board copy_board;
    if (isCastlingMove(curr_pos, next_pos, chessboard)) {
        copy_board = makeCastlingMove(curr_pos, next_pos, chessboard);
    }
    else {
        copy_board = makeCommonMove(chessboard, next_pos, curr_pos, figure);
    }
    if (!trick_mode && checkShah(copy_board, color))
        copy_board.states.shah = color;
    if (!trick_mode && checkShah(copy_board, FigInfo::not(color)))
        copy_board.states.shah = FigInfo::not(color);
    if (!trick_mode && chessboard.states.shah != EMPTY && copy_board.states.shah != EMPTY){
        cout << "shah detect" << endl;
        return false;
    }
    copy_board = PawnsPromotion::promoteAll(copy_board);
    chessboard = copy_board;
    return true;
}

Board Engine::makeCommonMove(const Board &chessboard, int next_pos, int curr_pos, int figure)
{
    Board copy_board = chessboard;
    if (copy_board.board[next_pos] != EMPTY) {
        int attacked_fig = 0;
        while (attacked_fig < NUMBER_OF_POSITIONS && copy_board.positions[attacked_fig] != next_pos)attacked_fig++;
        assert(copy_board.positions[attacked_fig] == next_pos);
        copy_board.positions[attacked_fig] = DESTROYED;
    }

    copy_board.positions[figure] = next_pos;
    copy_board.board[curr_pos] = EMPTY;
    copy_board.colors[curr_pos] = EMPTY;
    copy_board.board[next_pos] = FigInfo::getFigNumber(figure, figure / 16);
    copy_board.colors[next_pos] = FigInfo::getColor(figure);
    return copy_board;
}

int Engine::MarkPosition(const Board &position, int color){
   // GuiModule::printBoard(position, cout);
    int result = Valuator::i().materialValuation(position, color);
    GameState phase = Valuator::i().getGamePhase(position);
    if (phase == MATTING) {
        if (checkDraw(position, color))
            return -1000;
        else
            return result + Valuator::i().mattingPositionalValue(position, color);
    }
    result += PawnsValuator::getPositionalValue(position, color, phase);
    result += KnightsValuator::getPositionalValue(position, color);
    result += BishopsValuator::getPositionalValue(position, color, phase);
    result += RocksValuator::getPositionalValue(position, color, phase);
    result += QueenValuator::getPositionalValue(position, color, phase);
    result += KingsValuator::getPositionalValue(position, color, phase);
    return result;
}

int Engine::MarkMaterial(const Board &position, int color){
    return Valuator::i().materialValuation(position, color);
}

int Engine::AlfaBetaMinimax(int level, const Board &position, int color, int alfa, int beta, bool isMaximalizePhase){
    if (level == 0){
        int material = MarkMaterial(position, FigInfo::not(color));
        int result = ForcefulAlfaBeta(4, position, color, material, true) - material;
        //GuiModule::printBoard(position, cout);
        return result + MarkPosition(position, FigInfo::not(color));
    }
    vector<Board> available_positions;
    if (level==1)
        available_positions = Generator::GetAvailableMovements(position, color, true);
    else
        available_positions = Generator::GetAvailableMovements(position, color);
    //SortingPositions(position, available_moves)
    if (available_positions.size() == 0)
        return AlfaBetaMinimax(0, position, FigInfo::not(color), alfa, beta, isMaximalizePhase);
    for (Board next_position : available_positions){
        int tmp_value = AlfaBetaMinimax(level - 1, next_position, FigInfo::not(color), alfa, beta, !isMaximalizePhase);
        if (isMaximalizePhase){
            alfa = max(alfa, tmp_value);
        }
        else{
            beta = min(beta, tmp_value);
        }
        if (beta <= alfa) {
            //GuiModule::printBoard(position, cout);
            return tmp_value;
        }
    }
    //GuiModule::printBoard(position, cout);
    return (isMaximalizePhase) ? alfa : beta;
}

int Engine::ForcefulAlfaBeta(int level, const Board &position, int color, int old_material, bool max){
    if (level == 0) return MarkMaterial(position, FigInfo::not(color));
    vector<Board> available_positions = Generator::GetAttackMovements(position, color);
    //SortingPositions(position, available_moves)
    int best = INT32_MIN, worst = INT32_MAX;
    if (available_positions.size() == 0)return old_material;

    for (Board &next_position : available_positions){
        int tmp_value = ForcefulAlfaBeta(level - 1, next_position, FigInfo::not(color), old_material, !max);
        if (max){
            //if (tmp_value > best)best = tmp_value;
            best = std::max(best, tmp_value);
        }
        else{
            if (tmp_value < worst)worst = tmp_value;
            if (old_material > tmp_value)return old_material;
        }
    }
    if (max)return best;
    return worst;
}

int Engine::isCastlingMove(int curr_pos, int next_pos, const Board &chessboard)
{
    int myColor = chessboard.colors[curr_pos];
    bool kingInitialCondition = (   chessboard.board[curr_pos] == K 
                                    && chessboard.states.figOnStartPos[FigInfo::getPosIndex(K, myColor)]);
    if (kingInitialCondition == false)
        return false;
    
    if (myColor == WHITE) {
        if (next_pos == 62) {
            bool freePath = chessboard.board[next_pos] == EMPTY && chessboard.board[next_pos - 1] == EMPTY;
            bool rookIsNeighbor = chessboard.board[next_pos + 1] == W;
            bool rookIsInStartPos = chessboard.states.figOnStartPos[FigInfo::getPosIndex(W, WHITE, 1)];
            return freePath && rookIsNeighbor && rookIsInStartPos;
        }
        else if (next_pos == 57) {
            bool freePath = chessboard.board[next_pos] == EMPTY && chessboard.board[next_pos + 1] == EMPTY  && chessboard.board[next_pos + 2] == EMPTY;
            bool rookIsNeighbor = chessboard.board[next_pos - 1] == W;
            bool rookIsInStartPos = chessboard.states.figOnStartPos[FigInfo::getPosIndex(W, WHITE, 0)];
            return freePath && rookIsNeighbor && rookIsInStartPos;
        }
    }
    else {
        if (next_pos == 6) {
            bool freePath = chessboard.board[next_pos] == EMPTY && chessboard.board[next_pos - 1] == EMPTY;
            bool rookIsNeighbor = chessboard.board[next_pos + 1] == W;
            bool rookIsInStartPos = chessboard.states.figOnStartPos[FigInfo::getPosIndex(W, BLACK, 1)];
            return freePath && rookIsNeighbor && rookIsInStartPos;
        }
        else if (next_pos == 57) {
            bool freePath = chessboard.board[next_pos] == EMPTY && chessboard.board[next_pos + 1] == EMPTY  && chessboard.board[next_pos + 2] == EMPTY;
            bool rookIsNeighbor = chessboard.board[next_pos - 1] == W;
            bool rookIsInStartPos = chessboard.states.figOnStartPos[FigInfo::getPosIndex(W, BLACK, 0)];
            return freePath && rookIsNeighbor && rookIsInStartPos;
        }
    }
    return false;
}

bool Engine::checkDraw(const Board & position, int color)
{
    return  position.positions[FigInfo::getPosIndex(K, color)] != DESTROYED 
            && !checkShah(position, color) 
            && Generator::GetAvailableMovements(position, FigInfo::not(color)).size() == 0;
}

Board Engine::makeCastlingMove(int curr_pos, int next_pos, Board & chessboard)
{
    Board copy_board = chessboard;
    int myColor = chessboard.colors[curr_pos];
    int kingPosIdx = FigInfo::getPosIndex(K, myColor);
    copy_board.positions[kingPosIdx] = next_pos;
    copy_board.board[curr_pos] = EMPTY;
    copy_board.colors[curr_pos] = EMPTY;
    copy_board.states.figOnStartPos[kingPosIdx] = false;
    copy_board.board[next_pos] = K;
    copy_board.colors[next_pos] = myColor;
    const int shiftToRight = 1, shiftToLeft = -1;
    if (next_pos == 62 || next_pos == 6) {
        int rookPosIdx = FigInfo::getPosIndex(W, myColor, 1);
        return moveRookOnCastling(rookPosIdx, copy_board, next_pos, shiftToRight);
    }
    else if (next_pos == 57 || next_pos == 1) {
        int rookPosIdx = FigInfo::getPosIndex(W, myColor, 0);
        return moveRookOnCastling(rookPosIdx, copy_board, next_pos, shiftToLeft);
    }
    else {
        throw runtime_error("something wrong with next pos on castling in user move");
    }

}

Board Engine::moveRookOnCastling(int rookPosIdx, Board &copy_board, int next_pos, int shiftToRockPos)
{
    int myColor = FigInfo::getColor(rookPosIdx);
    copy_board.positions[rookPosIdx] = next_pos - shiftToRockPos;
    copy_board.board[next_pos + shiftToRockPos] = EMPTY;
    copy_board.colors[next_pos + shiftToRockPos] = EMPTY;
    copy_board.states.figOnStartPos[rookPosIdx] = false;
    copy_board.board[next_pos - shiftToRockPos] = W;
    copy_board.colors[next_pos - shiftToRockPos] = rookPosIdx;
    return copy_board;
}

Board Engine::NormalAlfaBeta(Board &position, int color, int level) {
    vector<Board> available_positions = Generator::GetAvailableMovements(position, color);
    available_positions.pop_back(); //empty movement erase
    vector<Board> no_shah_moves;
    for (Board &next_pos : available_positions) {
        if (!checkShah(next_pos, color))
            no_shah_moves.push_back(next_pos);
    }
    available_positions = no_shah_moves;
    if (available_positions.size() == 0) {
        if (position.states.shah != EMPTY)
            cout << "CHECKMATE BLACK WIN" << endl;
        else
            cout << "STALEMATE NOBODY WIN" << endl;
        position.states.shah = END;
        return position;
    }
    int alfa = INT32_MIN, beta = INT32_MAX;
    vector<int> values;
    values.resize(available_positions.size());
    int i;
//#pragma omp parallel for default(none) private(i) shared(values, level, available_positions, color, alfa, beta)
    for (i = 0; i < static_cast<int>(available_positions.size()); i++) {
        values[i] = AlfaBetaMinimax(level - 1, available_positions[i], FigInfo::not(color), alfa, beta, false);
    }

    int best = INT32_MIN, best_id = 0;
    for (size_t idx = 0; idx < values.size(); idx++) {
        if (values[idx] >= best && !BoardRememberer::i().isRemembered(available_positions[idx])) {
            best = values[idx];
            best_id = idx;
        }
    }

    if (available_positions[best_id].states.shah == color)
        available_positions[best_id].states.shah = EMPTY;
#ifdef DEBUG
    for (int i = 0; i < values.size(); i++) {
        cout << values[i]<<endl;
        Statistics::printValues(available_positions[i], color);
        GuiModule::printBoard(available_positions[i], cout);
    }
#endif
    BoardRememberer::i().addBoard(available_positions[best_id]);
    return available_positions[best_id];
}


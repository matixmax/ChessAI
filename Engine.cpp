
#include <algorithm>
#include "Engine.h"
#include "Definitions.h"
#include "Generator.h"
#include "Valuator.h"
#include "Board.h"
#include "Globals.h"
#include "FigInfo.h"

using namespace std;

bool Engine::checkFiguresMovement(int curr_pos, int next_pos, const Board &chessboard){
    int8 figure = chessboard.board[curr_pos];
    int distance = 1;
    for (unsigned int move_id = 0; move_id < g_figMoves[figure].size() && distance < 8; move_id++){
        if (g_figMoves[figure][move_id] == RANGED_FAR){
            distance++;
            move_id = 0;
        }
        int previus_pos = curr_pos + (distance - 1) * g_figMoves[figure][move_id];
        int pos = curr_pos + distance * g_figMoves[figure][move_id];
        if (pos< 0 || pos >= 64 || abs(pos % 8 - previus_pos % 8) > 2 )continue;
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

bool Engine::checkShah(const Board &chessboard, int8 color){
    auto possibleAttacks = Generator::GetPosAttackMove(    chessboard,
                                chessboard.positions[FigInfo::getPosIndex(K, color)],
                                FigInfo::not(color),
                                true);
    if ( possibleAttacks.size() == 0 )
        return false;
    return true;
}

bool Engine::userMove(int curr_pos, int next_pos, Board &chessboard, int8 color, bool trick_mode){
    int figure = 0;
    if (curr_pos > 63 || curr_pos < 0 || next_pos > 63 || next_pos < 0 || chessboard.board[curr_pos] == EMPTY){
        cout << "bad position " << endl;
        return false;
    }
    while (chessboard.positions[figure] != curr_pos){
        figure++;
        if (figure >= 32){
            cout << "bad figure" << endl;
            return false;
        }
    }
    if (!trick_mode && !checkFiguresMovement(curr_pos, next_pos, chessboard)){
        cout << "bad movement" << endl;
        return false;
    }
    Board copy_board = chessboard;
    if (copy_board.board[next_pos] != EMPTY){
        if (copy_board.colors[next_pos] == copy_board.colors[curr_pos]){
            cout << "can't attack myself" << endl;
            return false;
        }
        int attacked_fig = 0;
        while (attacked_fig < 32 && copy_board.positions[attacked_fig] != next_pos)attacked_fig++;
        assert(copy_board.positions[attacked_fig] == next_pos);
        copy_board.positions[attacked_fig] = DESTROYED;
    }

    copy_board.positions[figure] = next_pos;
    copy_board.board[curr_pos] = EMPTY;
    copy_board.colors[curr_pos] = EMPTY;
    copy_board.board[next_pos] = FigInfo::getFigNumber(figure, figure / 16);
    copy_board.colors[next_pos] = figure / 16;
    if (!trick_mode && checkShah(copy_board, color))
        copy_board.states.shah = color;
    if (!trick_mode && checkShah(copy_board, FigInfo::not(color)))
        copy_board.states.shah = FigInfo::not(color);
    if (!trick_mode && chessboard.states.shah != EMPTY && copy_board.states.shah != EMPTY){
        cout << "shah detect" << endl;
        return false;
    }
    chessboard = copy_board;
    return true;
}

int Engine::MarkPosition(const Board &position, int8 color){
    //printBoard(position);
    int result = Valuator::i().materialValuation(position, WHITE);
    int phase = Valuator::i().checkGamePhase(position);
    if (phase == Valuator::MATTING)
        return result + Valuator::i().mattingPositionalValue(position, color);
    result += Valuator::i().pawnsPositionalValue(position, color, phase);
    result += Valuator::i().knightsPositionalValue(position, color);
    result += Valuator::i().bishopsPositionalValue(position, color, phase);
    result += Valuator::i().rooksPositionalValue(position, color, phase);
    result += Valuator::i().queenPositionalValue(position, color, phase);
    result += Valuator::i().kingPositionalValue(position, color, phase);
    return result;
}

int Engine::MarkMaterial(const Board &position, int8 color){
    return Valuator::i().materialValuation(position, color);
}

int Engine::AlfaBetaMinimax(int level, const Board &position, int8 color, int alfa, int beta, bool max){
    if (level == 0){
        int material = MarkMaterial(position, color);
        int result = ForcefulAlfaBeta(4, position, color, material, true) - material;
        return result + MarkPosition(position, WHITE);
    }
    vector<Board> available_positions;
    if (level==1)
        available_positions = Generator::GetAvailableMovements(position, color, true);
    else
        available_positions = Generator::GetAvailableMovements(position, color);
    //SortingPositions(position, available_moves)
    if (available_positions.size() == 0)return AlfaBetaMinimax(0, position, color, alfa, beta, max);
    for (Board next_position : available_positions){
        int tmp_value = AlfaBetaMinimax(level - 1, next_position, FigInfo::not(color), alfa, beta, !max);
        if (max){
            if (tmp_value >= beta)return tmp_value;
            if (tmp_value > alfa)alfa = tmp_value;
        }
        else{
            if (tmp_value <= alfa)return tmp_value;
            if (tmp_value < beta)beta = tmp_value;
        }
    }
    if (max)return alfa;
    return beta;
}

int Engine::ForcefulAlfaBeta(int level, const Board &position, int8 color, int old_material, bool max){
    if (level == 0) return MarkMaterial(position, WHITE);
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

Board Engine::NormalAlfaBeta(Board &position, int8 color, int level){
    vector<Board> available_positions = Generator::GetAvailableMovements(position, color);
    available_positions.pop_back(); //empty movement erase
    vector<Board> no_shah_moves;
    for (Board &next_pos : available_positions){
        if (!checkShah(next_pos, color))
            no_shah_moves.push_back(next_pos);
    }
    available_positions = no_shah_moves;
    if (available_positions.size() == 0){
        if (position.states.shah != EMPTY)
            cout << "CHECKMATE BLACK WIN" << endl;
        else
            cout << "STALEMATE NOBODY WIN" << endl;
        position.states.shah = END;
        return position;
    }
    int alfa = INT32_MIN, beta = INT32_MAX;
    int best = INT32_MIN, best_id = 0;
    vector<int> values;
    values.resize(available_positions.size());
    int i;
    #pragma omp parallel for default(none) private(i) shared(values, level, available_positions, color, alfa, beta)
    for (i = 0; i < static_cast<int>( available_positions.size() ); i++){
        values[i] = AlfaBetaMinimax(level - 1, available_positions[i], FigInfo::not(color), alfa, beta, false);
    }
    auto it = std::max_element(values.begin(), values.end());
    best_id = std::distance(values.begin(), it);
    if (available_positions[best_id].states.shah == color)
        available_positions[best_id].states.shah = EMPTY;
    return available_positions[best_id];
}


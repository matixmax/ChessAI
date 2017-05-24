#include <array>
#include "Definitions.h"
#include "GuiModule.h"
#include "Generator.h"
#include "Board.h"
#include "Globals.h"
#include "FigInfo.h"
#include "PawnsPromotion.h"

using namespace std;

Generator::Generator(vector<int> chessboard, vector<int> positions, vector<int> colors): m_chessboard(chessboard, positions, colors){
    m_currentFigure[BLACK] = K;          m_currentFigure[WHITE] = K;
    m_currentMove[BLACK] = 0;            m_currentMove[WHITE] = 0;
    m_currentMoveDistance[BLACK] = 1;    m_currentMoveDistance[WHITE] = 1;
    m_nextPos[BLACK] = -1;               m_nextPos[WHITE] = -1;
    m_attack[BLACK] = false;             m_attack[WHITE] = false;
    m_specialMoves[BLACK] = 0;           m_specialMoves[WHITE] = 0;
}

Generator::Generator(Board board): Generator(board.board, board.positions, board.colors){
}

Generator::Generator(const Generator &generator): m_chessboard(generator.m_chessboard){
    m_currentFigure[BLACK] = generator.m_currentFigure[BLACK];            
    m_currentFigure[WHITE] = generator.m_currentFigure[WHITE];
    m_currentMove[BLACK] = generator.m_currentMove[BLACK];            
    m_currentMove[WHITE] = generator.m_currentMove[WHITE];
    m_currentMoveDistance[BLACK] = generator.m_currentMoveDistance[BLACK];    
    m_currentMoveDistance[WHITE] = generator.m_currentMoveDistance[WHITE];
    m_nextPos[BLACK] = generator.m_nextPos[BLACK];                
    m_nextPos[WHITE] = generator.m_nextPos[WHITE];
    m_attack[BLACK] = generator.m_attack[BLACK];
    m_attack[WHITE] = generator.m_attack[WHITE];
    m_specialMoves[BLACK] = generator.m_specialMoves[BLACK];
    m_specialMoves[WHITE] = generator.m_specialMoves[WHITE];
}

Board Generator::GetNextPosition(int color){
    Board result(m_chessboard);
    if (m_nextPos[color] == -1)
        m_nextPos[color] = nextPos(color);
    if (m_nextPos[color] == -1)
        return result;
    int last_fig = m_currentFigure[color];
    if (m_currentMove[color] == 0 && m_currentMoveDistance[color] == 1)
        last_fig--;
    if (m_attack[color])
        destroyAttackedFig(result, m_nextPos[color], color);
    result.board[result.positions[last_fig + 16 * color]] = EMPTY;
    result.colors[result.positions[last_fig + 16 * color]] = EMPTY;
    result.board[m_nextPos[color]] = FigInfo::getFigNumber(last_fig + color *16, color);
    result.colors[m_nextPos[color]] = color;
    result.positions[last_fig + 16 * color] = m_nextPos[color];
    m_nextPos[color] = -1;
    return result;
}

bool Generator::HasNext(int color){
    if (m_nextPos[color] == -1)
        m_nextPos[color] = nextPos(color);
    return (m_nextPos[color] != -1);
}

vector<Board> Generator::GetAttackMovements(const Board &chessboard, int color){
    vector<Board> attack_moves, tmp;
    int figure;
    #pragma omp parallel for default(none) private(figure, tmp) shared(attack_moves, chessboard, color)
    for (figure = 16 * FigInfo::not(color); figure < 8 + FigInfo::not(color) * 16; figure++){
        if (chessboard.positions[figure] != DESTROYED && isNotAKing(color, figure)){
            tmp = GetOnPositionAttackMove(chessboard, chessboard.positions[figure], color);
            attack_moves.insert(attack_moves.end(), tmp.begin(), tmp.end());
        }
    }
    return attack_moves;

}

vector<Board> Generator::GetAvailableMovements(const Board &chessboard, int color, bool attack_movement){
    vector<Board> normal_moves;
    vector<Board> attack_moves;
    attack_moves = Generator::getAttackPawnMoves(chessboard, color);
    for (int figure : Generator::getFiguresOrder(chessboard, color)){
        int fig_move_idx = FigInfo::getFigNumber(figure, color);
        int curr_pos = chessboard.positions[figure];
        for (int move_idx = 0; move_idx < (int)g_figMoves[fig_move_idx].size(); move_idx++){
            if (g_figMoves[fig_move_idx][move_idx] == RANGED_FAR)continue;
            if (*g_figMoves[FigInfo::getFigNumber(figure, color)].rbegin() == RANGED_FAR){
                for (int distance = 1; distance < 8; distance++){
                    int movement = g_figMoves[fig_move_idx][move_idx] * distance;
                    bool atacked = false;
                    if (validateMovement(chessboard, curr_pos, movement, atacked, distance)){
                        if (atacked){
                            attack_moves.push_back(Generator::getBoard(chessboard, color, figure, curr_pos + movement));
                            break;
                        }
                        else if (!attack_movement)
                            normal_moves.push_back(Generator::getBoard(chessboard, color, figure, curr_pos + movement));
                    }
                    else
                        break;
                }
            }
            else{
                int movement = g_figMoves[fig_move_idx][move_idx];
                bool atacked = false;
                if (validateMovement(chessboard, curr_pos, movement, atacked, 1)){
                    if (atacked){
                        if (fig_move_idx >= 5)
                            continue;
                        attack_moves.push_back(Generator::getBoard(chessboard, color, figure, curr_pos + movement));
                    }
                    else if (!attack_movement)
                        normal_moves.push_back(Generator::getBoard(chessboard, color, figure, curr_pos + movement));
                }
            }
        }
    }
    if (attack_movement)
        return attack_moves;
    vector<Board> special_moves = Generator::getSpecialMoves(chessboard, color);
    normal_moves.insert(normal_moves.end(), special_moves.begin(), special_moves.end());
    attack_moves.insert(attack_moves.end(), normal_moves.begin(), normal_moves.end());
    attack_moves.push_back(chessboard);
    return attack_moves;
}

vector<Board> Generator::GetOnPositionAttackMove(const Board &chessboard, int target, int color, bool one_attack){
    vector<Board> available_moves[3];
    int target_x = target % 8, target_y = target / 8;
    vector<pair<int,int>> knights_moves = { {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2} };
    vector<pair<int, int>> distance_moves = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };

    available_moves[0] = getOnPositionPawnsAttack(chessboard, target, color, one_attack);

    int figure;
    for (int i = 0; i < 8; i++){
        int next_x = target_x + knights_moves[i].first;
        int next_y = target_y + knights_moves[i].second;
        if (next_x < 0 || next_x > 7 || next_y < 0 || next_y > 7)continue;
        if (chessboard.board[next_x + next_y * 8] == S && chessboard.colors[next_x + next_y * 8] == color){
            figure = getFigureFromPosition(chessboard, color, S, next_x + next_y * 8);
            available_moves[1].push_back(Generator::getBoard(chessboard, color, figure, target));
            if (one_attack)return available_moves[1];
        }
    }
    for (int i = 0; i < 8; i++){
        int next_x = target_x + distance_moves[i].first;
        int next_y = target_y + distance_moves[i].second;
        if (next_x < 0 || next_x > 7 || next_y < 0 || next_y > 7)continue;
        if (next_x + next_y * 8 == chessboard.positions[FigInfo::getPosIndex(K, color)]){
            available_moves[2].push_back(Generator::getBoard(chessboard, color, FigInfo::getPosIndex(K, color), target));
            if (one_attack)return available_moves[2];
        }
        for (int distance = 1; distance < 8; distance++){
            next_x = target_x + distance * distance_moves[i].first;
            next_y = target_y + distance * distance_moves[i].second;
            if (next_x < 0 || next_x > 7 || next_y < 0 || next_y > 7)break;
            int next_pos = next_x + next_y * 8;
            if (chessboard.board[next_pos] != EMPTY){
                if (chessboard.colors[next_pos] == color){
                    figure = searchAttackFigure(chessboard, next_pos, color, i);
                    if (figure != -1){
                        available_moves[2].push_back(Generator::getBoard(chessboard, color, figure, target));
                        if (one_attack)return available_moves[2];
                    }
                }
                break;
            }
        }
    }
    available_moves[2].insert(available_moves[2].end(), available_moves[1].begin(), available_moves[1].end());
    available_moves[2].insert(available_moves[2].end(), available_moves[0].begin(), available_moves[0].end());
    return available_moves[2];
}

vector<Board> Generator::getOnPositionPawnsAttack(const Board &chessboard, int target, int color, bool one_attack)
{
    vector<Board> available_moves;
    int target_x = target % 8, target_y = target / 8;
    int figure;
    if (color == WHITE && target_y != 7) {
        if (target_x != 7 && chessboard.board[target + 9] == Pb) {
            figure = getFigureFromPosition(chessboard, color, Pb, target + 9);
            available_moves.push_back(Generator::getBoard(chessboard, color, figure, target));
            if (one_attack)return available_moves;
        }
        if (target_x != 0 && chessboard.board[target + 7] == Pb) {
            figure = getFigureFromPosition(chessboard, color, Pb, target + 7);
            available_moves.push_back(Generator::getBoard(chessboard, color, figure, target));
            if (one_attack)return available_moves;
        }
    }
    if (color == BLACK && target_y != 0) {
        if (target_x != 7 && chessboard.board[target - 7] == Pc) {
            figure = getFigureFromPosition(chessboard, color, Pc, target - 7);
            available_moves.push_back(Generator::getBoard(chessboard, color, figure, target));
            if (one_attack)return available_moves;
        }
        if (target_x != 0 && chessboard.board[target - 9] == Pc) {
            figure = getFigureFromPosition(chessboard, color, Pc, target - 9);
            available_moves.push_back(Generator::getBoard(chessboard, color, figure, target));
            if (one_attack)return available_moves;
        }
    }
    return available_moves;
}

Board Generator::getBoard(const Board &chessboard, int color, int figure, int next_pos){
    Board result = chessboard;
    if (result.board[next_pos] != EMPTY)
        Generator::staticDestroyAttackedFig(result, next_pos, color, next_pos);
    result.board[result.positions[figure]] = EMPTY;
    result.colors[result.positions[figure]] = EMPTY;
    result.board[next_pos] = FigInfo::getFigNumber(figure, color);
    result.colors[next_pos] = color;
    result.positions[figure] = next_pos;
    result = PawnsPromotion::promoteAll(result);
    return result;
}

bool Generator::pawnCondition(int color, int next_pos, bool double_move){
    if (double_move){
        if (m_chessboard.board[next_pos - ((-color * 2) - 1) * 8] != EMPTY ||
                m_chessboard.board[next_pos] != EMPTY)
            return false;
        if (m_chessboard.board[next_pos + 1] == Pb - color ||
                m_chessboard.board[next_pos - 1] == Pb - color) // beating in passing
            int beating_in_passing = 1;
        return true;
    }
    if (m_currentFigure[color] < 8)//its no pawn
        return true;
    if (m_specialMoves[color] == 0){
        m_attack[color] = false; // this pawn can't attack
        return !(m_chessboard.board[next_pos] != EMPTY);
    }
    return !(m_chessboard.colors[next_pos] != (color ^ 1));
}

bool Generator::validateMovement(int curr_pos, int move, bool& attacked){
    int distance = (m_chessboard.colors[curr_pos] != EMPTY) 
        ? m_currentMoveDistance[m_chessboard.colors[curr_pos]] 
        : 1;
    return validateMovement(m_chessboard, curr_pos, move, attacked, distance);
}

//validation of physical opportunity of move no check logic ex. pawn maked 3 steps is validate true
bool Generator::validateMovement(const Board &chessboard, int curr_pos, int move, bool& attacked, int distance){
    bool local_attack_flag = false;
    if (move == RANGED_FAR)
        return false;
    if (chessboard.board[curr_pos] == EMPTY)
        return false;
    int next_pos = curr_pos + move;
    if (next_pos < 0 || next_pos > 63)
        return false;
    if (chessboard.board[next_pos] != EMPTY){
        if (chessboard.colors[next_pos] != chessboard.colors[curr_pos])
            local_attack_flag = true;
        else
            return false;
    }
    int direction = (move / distance) % 8;
    if (direction < 0)direction += 8;
    if (direction > 5){//movement to the left
        if (curr_pos % 8 < (move + curr_pos) % 8)
            return false;
    }
    else{//movement to the right
        if (curr_pos % 8 > (move + curr_pos) % 8)
            return false;
    }
    if (chessboard.positions[0] == DESTROYED || chessboard.positions[16] == DESTROYED)
        return false;
    if (local_attack_flag)
        attacked = true;
    return true;
}

int Generator::getFigureFromPosition(const Board &chessboard, int color, int figure, int position){
    int figure_pos;
    for (int number = 0; number < 8; number++){
        figure_pos = FigInfo::getPosIndex(figure, color, number);
        if (chessboard.positions[figure_pos] == position)
            break;
    }
    assert(chessboard.positions[figure_pos] == position);
    return figure_pos;
}

int Generator::manageSpecialMoves(int color){
    manageSpecialMoves_begin: //goto statement
    if (m_specialMoves[color] <= 8){// pawns double move
        int fig = m_specialMoves[color] + 7 + color * 16;
        int curr_pos = m_chessboard.positions[fig];
        int next_move = curr_pos + ((-color * 2) - 1) * 16;
        m_specialMoves[color]++;
        if (!(curr_pos / 8 == color * 5 + 1) || !pawnCondition(color, next_move, true))
            goto manageSpecialMoves_begin;
        m_currentFigure[color] = fig;
        m_currentMove[color] = 1; // set because in GetNextPosition is condition whose check current move = 0
    }
    if (m_specialMoves[color] == 9){ //left castling
        //very big condition to late to write this
    }
    return -1;
}

int Generator::nextPos(int color){
    nextPos_begin: //goto statement
    if (m_currentFigure[color] > 15){
        if (m_specialMoves[color] == 0){
            m_specialMoves[color] = 1;        //phase of the pawns attack
            m_currentFigure[color] -= 8;
        }
        else
            return manageSpecialMoves(color);
    }
    int attack_pawns = m_specialMoves[color] * 2;
    int fig = m_currentFigure[color] + color * 16;
    int curr_pos = m_chessboard.positions[fig];
    int fig_move_idx = FigInfo::getFigNumber(fig, color) + attack_pawns;
    int movement = g_figMoves[fig_move_idx][m_currentMove[color]] * m_currentMoveDistance[color];
    bool attacked = false;
    if (curr_pos == DESTROYED || !validateMovement(curr_pos, movement, attacked) || !pawnCondition(color, curr_pos + movement)){
        slideMovement(fig, fig_move_idx);
        goto nextPos_begin;
        //return nextPos(color);
    }
    slideMovement(fig, fig_move_idx, true);
    return curr_pos + movement;
}

int Generator::searchAttackFigure(const Board &chessboard, int next_pos, int color, int move_nb){
    int figure = chessboard.board[next_pos];
    switch (figure){
    case H:
        figure = FigInfo::getPosIndex(figure, color);
        break;
    case W:
        if (move_nb % 2 == 0){
            figure = getFigureFromPosition(chessboard, color, figure, next_pos);
        }
        else figure = -1;
        break;
    case G:
        if (move_nb % 2 == 1){
            figure = getFigureFromPosition(chessboard, color, figure, next_pos);
        }
        else figure = -1;
        break;
    default:
        figure = -1;
        break;
    }
    return figure;
}

vector<Board> Generator::getAttackPawnMoves(const Board &chessboard, int color){
    vector<Board> attack_moves;
    for (int figure = 8; figure < 16; figure++){
        for (int i = 0; i < 2; i++){
            int movement = g_figMoves[7 + color][i];
            int curr_pos = chessboard.positions[figure + 16 * color];
            bool atacked = false;
            if (curr_pos != DESTROYED && validateMovement(chessboard, curr_pos, movement, atacked, 1)){
                if (atacked)
                    attack_moves.push_back(Generator::getBoard(chessboard, color, figure + 16 * color, curr_pos + movement));
            }
        }
    }
    return attack_moves;
}

vector<Board> Generator::getSpecialMoves(const Board &chessboard, int color){
    vector<Board> double_moves;
    for (int figure = 8; figure < 16; figure++){
        int curr_pos = chessboard.positions[figure + 16 * color];
        int next_move = curr_pos - ((color * 2) - 1) * 16;
        bool atacked = false;
        if ((curr_pos / 8 == color * 5 + 1) &&
                chessboard.board[next_move + ((color * 2) - 1) * 8] == EMPTY &&
                chessboard.board[next_move] == EMPTY)
            double_moves.push_back(Generator::getBoard(chessboard, color, figure + 16 * color, next_move));
        
    }
    return double_moves;
}

vector<int> Generator::getFiguresOrder(const Board &chessboard, int color){
    vector<int> pos_order = { 27, 28, 35, 36, 18, 19, 20, 21, 26, 29, 34, 37, 42, 43, 44, 45,
        9, 10, 11, 12, 13, 14, 17, 22, 25, 30, 33, 38, 41, 46, 49, 50,
        51, 52, 53, 54, 0, 1, 2, 3, 4, 5, 6, 7, 56, 57, 58, 59, 60, 61,
        62, 63, 8, 15, 16, 23, 24, 31, 32, 39, 40, 47, 48, 55 };
    vector<int> figures_order;
    for (unsigned pos = 0; pos < pos_order.size(); pos++){
        for (int j = 16 * color; j < 16 + 16 * color; j++){
            if (chessboard.positions[j] == pos_order[pos])
                figures_order.push_back(j);
        }
    }
    return figures_order;
}

void Generator::destroyAttackedFig(Board &board, int pos, int color, int nextPos){
    if (nextPos == -1){//sequential mode
        if (m_currentMoveDistance[color] != 1){
            int fig = m_currentFigure[color] + color * 16;
            slideMovement(fig, FigInfo::getFigNumber(fig, color));
        }
        m_attack[color] = false;
        nextPos = m_nextPos[color];
    }
    int attacked_fig = FigInfo::getPosIndex(board.board[nextPos], (color + 1) % 2);
    int number = 1;
    while (board.positions[attacked_fig] != nextPos && number <= 8){
        attacked_fig = FigInfo::getPosIndex(board.board[nextPos], (color + 1) % 2, number);
        number++;
    }

    if (board.positions[attacked_fig] != nextPos)
        runtime_error("figure attack not found");

    board.positions[attacked_fig] = DESTROYED;
}

bool Generator::isNotAKing(int color, int posIdx)
{
    return FigInfo::getFigNumber(posIdx, color) != K;
}

void Generator::staticDestroyAttackedFig(Board &board, int pos, int color, int next_pos){
    int attacked_fig = FigInfo::getPosIndex(board.board[next_pos], FigInfo::not(color));
    if (board.board[next_pos] == K)
        board.states.shah = FigInfo::not(color);
    int number = 1;
    while (board.positions[attacked_fig] != next_pos && number < 8){
        attacked_fig = FigInfo::getPosIndex(board.board[next_pos], FigInfo::not(color), number);
        number++;
    }
    assert(board.positions[attacked_fig] == next_pos);
    board.positions[attacked_fig] = DESTROYED;
}

void Generator::slideMovement(int fig, int fig_move_idx, bool increase_range){
    int color = fig / 16;
    if (increase_range && *g_figMoves[FigInfo::getFigNumber(fig,color)].rbegin() == RANGED_FAR && m_currentMoveDistance[color] < 8){
        m_currentMoveDistance[color]++;
        return;
    }
    m_currentMoveDistance[color] = 1;
    if (m_currentMove[color] < g_figMoves[fig_move_idx].size() - 1){
        m_currentMove[color]++;
        return;
    }
    m_currentMove[color] = 0;
    m_currentFigure[color]++;
}

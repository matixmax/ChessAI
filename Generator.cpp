
#include "Definitions.h"
#include "GuiModule.h"
#include "Generator.h"
#include "Board.h"
#include "Globals.h"
#include "FigInfo.h"

using namespace std;

Generator::Generator(vector<int8> chessboard, vector<int8> positions, vector<int8> colors): m_chessboard(chessboard, positions, colors){
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

Board Generator::GetNextPosition(int8 color){
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

bool Generator::HasNext(int8 color){
    if (m_nextPos[color] == -1)
        m_nextPos[color] = nextPos(color);
    return (m_nextPos[color] != -1);
}

vector<Board> Generator::GetAttackMovements(const Board &chessboard, int8 color){
    vector<Board> attack_moves, tmp;
    int figure;
    #pragma omp parallel for default(none) private(figure, tmp) shared(attack_moves, chessboard, color)
    for (figure = 16 * FigInfo::not(color); figure < 8 + FigInfo::not(color) * 16; figure++){
        if (chessboard.positions[figure] != DESTROYED){
            tmp = GetPosAttackMove(chessboard, chessboard.positions[figure], color);
            attack_moves.insert(attack_moves.end(), tmp.begin(), tmp.end());
        }
    }
    return attack_moves;

}

vector<Board> Generator::GetAvailableMovements(const Board &chessboard, int8 color, bool attack_movement){
    vector<Board> normal_moves;
    vector<Board> attack_moves;
    attack_moves = Generator::getAttackPawnMoves(chessboard, color);
    for (int figure : Generator::getFiguresOrder(chessboard, color)){
        int8 fig_move_idx = FigInfo::getFigNumber(figure, color);
        int8 curr_pos = chessboard.positions[figure];
        for (int8 move_idx = 0; move_idx < (int8)g_figMoves[fig_move_idx].size(); move_idx++){
            if (g_figMoves[fig_move_idx][move_idx] == RANGED_FAR)continue;
            if (*g_figMoves[FigInfo::getFigNumber(figure, color)].rbegin() == RANGED_FAR){
                for (int8 distance = 1; distance < 8; distance++){
                    int8 movement = g_figMoves[fig_move_idx][move_idx] * distance;
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
                int8 movement = g_figMoves[fig_move_idx][move_idx];
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

vector<Board> Generator::GetPosAttackMove(const Board &chessboard, int8 target, int8 color, bool one_attack){
    vector<Board> available_moves[3];
    int8 target_x = target % 8, target_y = target / 8;
    int8 knights_moves[] = { 1, -2, 2, -1, 2, 1, 1, 2, -1, 2, -2, 1, -2, -1, -1, -2 };
    int8 distance_moves[] = { 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0, -1, -1 };
    int8 figure;
    if (color == WHITE && target_y != 7){
        if (target_x != 7 && chessboard.board[target + 9] == Pb){
            figure = getFigureFromPosition(chessboard, color, Pb, target + 9);
            available_moves[0].push_back(Generator::getBoard(chessboard, color, figure, target));
            if (one_attack)return available_moves[0];
        }
        if (target_x != 0 && chessboard.board[target + 7] == Pb){
            figure = getFigureFromPosition(chessboard, color, Pb, target + 7);
            available_moves[0].push_back(Generator::getBoard(chessboard, color, figure, target));
            if (one_attack)return available_moves[0];
        }
    }
    if (color == BLACK && target_y != 0){
        if (target_x != 7 && chessboard.board[target - 7] == Pc){
            figure = getFigureFromPosition(chessboard, color, Pc, target - 7);
            available_moves[0].push_back(Generator::getBoard(chessboard, color, figure, target));
            if (one_attack)return available_moves[0];
        }
        if (target_x != 0 && chessboard.board[target - 9] == Pc){
            figure = getFigureFromPosition(chessboard, color, Pc, target - 9);
            available_moves[0].push_back(Generator::getBoard(chessboard, color, figure, target));
            if (one_attack)return available_moves[0];
        }
    }
    for (int i = 0; i < 8; i++){
        int next_x = target_x + knights_moves[2 * i], next_y = target_y + knights_moves[2 * i + 1];
        if (next_x < 0 || next_x > 7 || next_y < 0 || next_y > 7)continue;
        if (chessboard.board[next_x + next_y * 8] == S && chessboard.colors[next_x + next_y * 8] == color){
            figure = getFigureFromPosition(chessboard, color, S, next_x + next_y * 8);
            available_moves[1].push_back(Generator::getBoard(chessboard, color, figure, target));
            if (one_attack)return available_moves[1];
        }
    }
    for (int i = 0; i < 8; i++){
        int next_x = target_x + distance_moves[2 * i], next_y = target_y + distance_moves[2 * i + 1];
        if (next_x < 0 || next_x > 7 || next_y < 0 || next_y > 7)continue;
        if (next_x + next_y * 8 == chessboard.positions[FigInfo::getPosIndex(K, color)]){
            available_moves[2].push_back(Generator::getBoard(chessboard, color, FigInfo::getPosIndex(K, color), target));
            if (one_attack)return available_moves[2];
        }
        for (int distance = 1; distance < 8; distance++){
            next_x = target_x + distance * distance_moves[2 * i];
            next_y = target_y + distance * distance_moves[2 * i + 1];
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

Board Generator::getBoard(const Board &chessboard, int8 color, int8 figure, int8 next_pos){
    Board result = chessboard;
    if (result.board[next_pos] != EMPTY)
        Generator::staticDestroyAttackedFig(result, next_pos, color, next_pos);
    result.board[result.positions[figure]] = EMPTY;
    result.colors[result.positions[figure]] = EMPTY;
    result.board[next_pos] = FigInfo::getFigNumber(figure, color);
    result.colors[next_pos] = color;
    result.positions[figure] = next_pos;
    return result;
}

bool Generator::pawnCondition(int8 color, int8 next_pos, bool double_move){
    if (double_move){
        if (m_chessboard.board[next_pos - ((-color * 2) - 1) * 8] != EMPTY ||
                m_chessboard.board[next_pos] != EMPTY)
            return false;
        if (m_chessboard.board[next_pos + 1] == Pb - color ||
                m_chessboard.board[next_pos - 1] == Pb - color) // beating in passing
            int8 beating_in_passing = 1;
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

bool Generator::validateMovement(int8 curr_pos, int8 move, bool& attacked){
    bool local_attack_flag = false;
    if (move == RANGED_FAR)
        return false;
    int8 next_pos = curr_pos + move;
    if (next_pos < 0 || next_pos > 63)
        return false;
    if (m_chessboard.board[next_pos] != EMPTY){
        if (m_chessboard.colors[next_pos] != m_chessboard.colors[curr_pos])
            local_attack_flag = true;
        else
            return false;
    }
    int8 direction = (move / m_currentMoveDistance[m_chessboard.colors[curr_pos]])%8;
    if (direction < 0)direction += 8;
    if (direction > 5){//movement to the left
        if (curr_pos % 8 < (move + curr_pos) % 8)
            return false;
    }
    else{//movement to the right
        if (curr_pos % 8 > (move + curr_pos) % 8)
            return false;
    }
    if (local_attack_flag){
        m_attack[m_chessboard.colors[curr_pos]] = true;
        attacked = true;
    }
    return true;
}

bool Generator::validateMovement(const Board &chessboard, int8 curr_pos, int8 move, bool& attacked, int8 distance){
    bool local_attack_flag = false;
    if (move == RANGED_FAR)
        return false;
    int8 next_pos = curr_pos + move;
    if (next_pos < 0 || next_pos > 63)
        return false;
    if (chessboard.board[next_pos] != EMPTY){
        if (chessboard.colors[next_pos] != chessboard.colors[curr_pos])
            local_attack_flag = true;
        else
            return false;
    }
    int8 direction = (move / distance) % 8;
    if (direction < 0)direction += 8;
    if (direction > 5){//movement to the left
        if (curr_pos % 8 < (move + curr_pos) % 8)
            return false;
    }
    else{//movement to the right
        if (curr_pos % 8 > (move + curr_pos) % 8)
            return false;
    }
    if (local_attack_flag)
        attacked = true;
    return true;
}

int8 Generator::getFigureFromPosition(const Board &chessboard, int8 color, int8 figure, int8 position){
    int8 figure_pos;
    for (int number = 0; number < 8; number++){
        figure_pos = FigInfo::getPosIndex(figure, color, number);
        if (chessboard.positions[figure_pos] == position)
            break;
    }
    assert(chessboard.positions[figure_pos] == position);
    return figure_pos;
}

int8 Generator::manageSpecialMoves(int8 color){
    manageSpecialMoves_begin: //goto statement
    if (m_specialMoves[color] <= 8){// pawns double move
        int8 fig = m_specialMoves[color] + 7 + color * 16;
        int8 curr_pos = m_chessboard.positions[fig];
        int8 next_move = curr_pos + ((-color * 2) - 1) * 16;
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

int8 Generator::nextPos(int8 color){
    nextPos_begin: //goto statement
    if (m_currentFigure[color] > 15){
        if (m_specialMoves[color] == 0){
            m_specialMoves[color] = 1;        //phase of the pawns attack
            m_currentFigure[color] -= 8;
        }
        else
            return manageSpecialMoves(color);
    }
    int8 attack_pawns = m_specialMoves[color] * 2;
    int8 fig = m_currentFigure[color] + color * 16;
    int8 curr_pos = m_chessboard.positions[fig];
    int8 fig_move_idx = FigInfo::getFigNumber(fig, color) + attack_pawns;
    int8 movement = g_figMoves[fig_move_idx][m_currentMove[color]] * m_currentMoveDistance[color];
    bool attacked = false;
    if (curr_pos == DESTROYED || !validateMovement(curr_pos, movement, attacked) || !pawnCondition(color, curr_pos + movement)){
        slideMovement(fig, fig_move_idx);
        goto nextPos_begin;
        //return nextPos(color);
    }
    slideMovement(fig, fig_move_idx, true);
    #ifdef DEBUG
        print((int)fig); print((int)curr_pos); print((int)movement); printTab((int)m_chessboard.positions, 32);
    #endif
    return curr_pos + movement;
}

int8 Generator::searchAttackFigure(const Board &chessboard, int8 next_pos, int8 color, int move_nb){
    int8 figure = chessboard.board[next_pos];
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

vector<Board> Generator::getAttackPawnMoves(const Board &chessboard, int8 color){
    vector<Board> attack_moves;
    for (int8 figure = 8; figure < 16; figure++){
        for (int8 i = 0; i < 2; i++){
            int8 movement = g_figMoves[7 + color][i];
            int8 curr_pos = chessboard.positions[figure + 16 * color];
            bool atacked = false;
            if (curr_pos != DESTROYED && validateMovement(chessboard, curr_pos, movement, atacked, 1)){
                if (atacked)
                    attack_moves.push_back(Generator::getBoard(chessboard, color, figure + 16 * color, curr_pos + movement));
            }
        }
    }
    return attack_moves;
}

vector<Board> Generator::getSpecialMoves(const Board &chessboard, int8 color){
    vector<Board> double_moves;
    for (int8 figure = 8; figure < 16; figure++){
        int8 curr_pos = chessboard.positions[figure + 16 * color];
        int8 next_move = curr_pos - ((color * 2) - 1) * 16;
        bool atacked = false;
        if ((curr_pos / 8 == color * 5 + 1) &&
                chessboard.board[next_move + ((color * 2) - 1) * 8] == EMPTY &&
                chessboard.board[next_move] == EMPTY)
            double_moves.push_back(Generator::getBoard(chessboard, color, figure + 16 * color, next_move));
        
    }
    return double_moves;
}

vector<int8> Generator::getFiguresOrder(const Board &chessboard, int8 color){
    vector<int8> pos_order = { 27, 28, 35, 36, 18, 19, 20, 21, 26, 29, 34, 37, 42, 43, 44, 45,
        9, 10, 11, 12, 13, 14, 17, 22, 25, 30, 33, 38, 41, 46, 49, 50,
        51, 52, 53, 54, 0, 1, 2, 3, 4, 5, 6, 7, 56, 57, 58, 59, 60, 61,
        62, 63, 8, 15, 16, 23, 24, 31, 32, 39, 40, 47, 48, 55 };
    vector<int8> figures_order;
    for (unsigned pos = 0; pos < pos_order.size(); pos++){
        for (int8 j = 16 * color; j < 16 + 16 * color; j++){
            if (chessboard.positions[j] == pos_order[pos])
                figures_order.push_back(j);
        }
    }
    return figures_order;
}

void Generator::destroyAttackedFig(Board &board, int8 pos, int8 color, int8 nextPos){
    if (nextPos == -1){//sequential mode
        if (m_currentMoveDistance[color] != 1){
            int8 fig = m_currentFigure[color] + color * 16;
            slideMovement(fig, FigInfo::getFigNumber(fig, color));
        }
        m_attack[color] = false;
        nextPos = m_nextPos[color];
    }
    int8 attacked_fig = FigInfo::getPosIndex(board.board[nextPos], (color + 1) % 2);
    int8 number = 1;
    while (board.positions[attacked_fig] != nextPos && number <= 8){
        attacked_fig = FigInfo::getPosIndex(board.board[nextPos], (color + 1) % 2, number);
        number++;
    }

    if (board.positions[attacked_fig] != nextPos)
        runtime_error("figure attack not found");

    board.positions[attacked_fig] = DESTROYED;
}

void Generator::staticDestroyAttackedFig(Board &board, int8 pos, int8 color, int8 next_pos){
    int8 attacked_fig = FigInfo::getPosIndex(board.board[next_pos], FigInfo::not(color));
    if (board.board[next_pos] == K)
        board.states.shah = FigInfo::not(color);
    int8 number = 1;
    while (board.positions[attacked_fig] != next_pos && number < 8){
        attacked_fig = FigInfo::getPosIndex(board.board[next_pos], FigInfo::not(color), number);
        number++;
    }
    assert(board.positions[attacked_fig] == next_pos);
    board.positions[attacked_fig] = DESTROYED;
}

void Generator::slideMovement(int8 fig, int8 fig_move_idx, bool increase_range){
    int8 color = fig / 16;
    if (increase_range && *g_figMoves[FigInfo::getFigNumber(fig,color)].rbegin() == RANGED_FAR && m_currentMoveDistance[color] < 8){
        m_currentMoveDistance[color]++;
        return;
    }
    m_currentMoveDistance[color] = 1;
    if (m_currentMove[color] < (int8)g_figMoves[fig_move_idx].size() - 1){
        m_currentMove[color]++;
        return;
    }
    m_currentMove[color] = 0;
    m_currentFigure[color]++;
}

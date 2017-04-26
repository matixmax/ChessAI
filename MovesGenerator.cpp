
#include "Definitions.h"
#include "Functions.h"
#include "Generator.h"

Generator::Generator(vector<int8> chessboard, vector<int8> positions, vector<int8> colors): chessboard(chessboard, positions, colors){
	current_figure[BLACK] = K;			current_figure[WHITE] = K;
	current_move[BLACK] = 0;			current_move[WHITE] = 0;
	current_move_distance[BLACK] = 1;	current_move_distance[WHITE] = 1;
	next_pos[BLACK] = -1;				next_pos[WHITE] = -1;
	attack[BLACK] = false;				attack[WHITE] = false;
	special_moves[BLACK] = 0;			special_moves[WHITE] = 0;
}

Generator::Generator(Board board): Generator(board.board, board.positions, board.colors){
}

Generator::Generator(const Generator &generator): chessboard(generator.chessboard){
	current_figure[BLACK] = generator.current_figure[BLACK];			
	current_figure[WHITE] = generator.current_figure[WHITE];
	current_move[BLACK] = generator.current_move[BLACK];			
	current_move[WHITE] = generator.current_move[WHITE];
	current_move_distance[BLACK] = generator.current_move_distance[BLACK];	
	current_move_distance[WHITE] = generator.current_move_distance[WHITE];
	next_pos[BLACK] = generator.next_pos[BLACK];				
	next_pos[WHITE] = generator.next_pos[WHITE];
	attack[BLACK] = generator.attack[BLACK];
	attack[WHITE] = generator.attack[WHITE];
	special_moves[BLACK] = generator.special_moves[BLACK];
	special_moves[WHITE] = generator.special_moves[WHITE];
}

Board Generator::GetNextPosition(int8 color){
	Board result(this->chessboard);
	if (this->next_pos[color] == -1)
		this->next_pos[color] = nextPos(color);
	if (this->next_pos[color] == -1)
		return result;
	int last_fig = this->current_figure[color];
	if (this->current_move[color] == 0 && this->current_move_distance[color] == 1)
		last_fig--;
	if (this->attack[color])
		destroyAttackedFig(result, this->next_pos[color], color);
	result.board[result.positions[last_fig + 16 * color]] = EMPTY;
	result.colors[result.positions[last_fig + 16 * color]] = EMPTY;
	result.board[this->next_pos[color]] = getFigureNbFromPosIdx(last_fig + color *16, color);
	result.colors[this->next_pos[color]] = color;
	result.positions[last_fig + 16 * color] = this->next_pos[color];
	this->next_pos[color] = -1;
	return result;
}

bool Generator::HasNext(int8 color){
	if (this->next_pos[color] == -1)
		this->next_pos[color] = nextPos(color);
	return (this->next_pos[color] != -1);
}

vector<Board> Generator::GetAttackMovements(const Board &chessboard, int8 color){
    vector<Board> attack_moves, tmp;
    int figure;
    #pragma omp parallel for default(none) private(figure, tmp) shared(attack_moves, chessboard, color)
    for (figure = 16 * not(color); figure < 8 + not(color) * 16; figure++){
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
        int8 fig_move_idx = getFigureNbFromPosIdx(figure, color);
        int8 curr_pos = chessboard.positions[figure];
        for (int8 move_idx = 0; move_idx < (int8)moves[fig_move_idx].size(); move_idx++){
            if (moves[fig_move_idx][move_idx] == RANGED_FAR)continue;
            if (*moves[getFigureNbFromPosIdx(figure, color)].rbegin() == RANGED_FAR){
                for (int8 distance = 1; distance < 8; distance++){
                    int8 movement = moves[fig_move_idx][move_idx] * distance;
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
                int8 movement = moves[fig_move_idx][move_idx];
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
        if (next_x + next_y * 8 == chessboard.positions[getPositionIndex(K, color)]){
            available_moves[2].push_back(Generator::getBoard(chessboard, color, getPositionIndex(K, color), target));
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
    result.board[next_pos] = getFigureNbFromPosIdx(figure, color);
    result.colors[next_pos] = color;
    result.positions[figure] = next_pos;
    return result;
}

bool Generator::pawnCondition(int8 color, int8 next_pos, bool double_move){
	if (double_move){
		if (this->chessboard.board[next_pos - ((-color * 2) - 1) * 8] != EMPTY ||
				this->chessboard.board[next_pos] != EMPTY)
			return false;
		if (this->chessboard.board[next_pos + 1] == Pb - color ||
				this->chessboard.board[next_pos - 1] == Pb - color) // beating in passing
			int8 beating_in_passing = 1;
		return true;
	}
	if (this->current_figure[color] < 8)//its no pawn
		return true;
	if (this->special_moves[color] == 0){
		this->attack[color] = false; // this pawn can't attack
		return !(this->chessboard.board[next_pos] != EMPTY);
	}
	return !(this->chessboard.colors[next_pos] != (color ^ 1));
}

bool Generator::validateMovement(int8 curr_pos, int8 move, bool& attacked){
    bool local_attack_flag = false;
	if (move == RANGED_FAR)
		return false;
	int8 next_pos = curr_pos + move;
	if (next_pos < 0 || next_pos > 63)
		return false;
	if (this->chessboard.board[next_pos] != EMPTY){
		if (this->chessboard.colors[next_pos] != this->chessboard.colors[curr_pos])
			local_attack_flag = true;
		else
			return false;
	}
    int8 direction = (move / this->current_move_distance[this->chessboard.colors[curr_pos]])%8;
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
        attack[this->chessboard.colors[curr_pos]] = true;
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
        figure_pos = getPositionIndex(figure, color, number);
        if (chessboard.positions[figure_pos] == position)
            break;
    }
    assert(chessboard.positions[figure_pos] == position);
    return figure_pos;
}

int8 Generator::manageSpecialMoves(int8 color){
	manageSpecialMoves_begin: //goto statement
	if (this->special_moves[color] <= 8){// pawns double move
		int8 fig = this->special_moves[color] + 7 + color * 16;
		int8 curr_pos = this->chessboard.positions[fig];
		int8 next_move = curr_pos + ((-color * 2) - 1) * 16;
		this->special_moves[color]++;
		if (!(curr_pos / 8 == color * 5 + 1) || !pawnCondition(color, next_move, true))
			goto manageSpecialMoves_begin;
		this->current_figure[color] = fig;
		this->current_move[color] = 1; // set because in GetNextPosition is condition whose check current move = 0
	}
	if (this->special_moves[color] == 9){ //left castling
		//very big condition to late to write this
	}
	return -1;
}

int8 Generator::nextPos(int8 color){
	nextPos_begin: //goto statement
	if (this->current_figure[color] > 15){
		if (this->special_moves[color] == 0){
			this->special_moves[color] = 1;		//phase of the pawns attack
			this->current_figure[color] -= 8;
		}
		else
			return manageSpecialMoves(color);
	}
	int8 attack_pawns = this->special_moves[color] * 2;
	int8 fig = this->current_figure[color] + color * 16;
	int8 curr_pos = this->chessboard.positions[fig];
	int8 fig_move_idx = getFigureNbFromPosIdx(fig, color) + attack_pawns;
	int8 movement = moves[fig_move_idx][this->current_move[color]] * this->current_move_distance[color];
    bool attacked = false;
    if (curr_pos == DESTROYED || !validateMovement(curr_pos, movement, attacked) || !pawnCondition(color, curr_pos + movement)){
		this->slideMovement(fig, fig_move_idx);
		goto nextPos_begin;
		//return nextPos(color);
	}
	this->slideMovement(fig, fig_move_idx, true);
	#ifdef DEBUG
		print((int)fig); print((int)curr_pos); print((int)movement); printTab((int)this->chessboard.positions, 32);
	#endif
	return curr_pos + movement;
}

int8 Generator::searchAttackFigure(const Board &chessboard, int8 next_pos, int8 color, int move_nb){
    int8 figure = chessboard.board[next_pos];
    switch (figure){
    case H:
        figure = getPositionIndex(figure, color);
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
            int8 movement = moves[7 + color][i];
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
        if (this->current_move_distance[color] != 1){
            int8 fig = this->current_figure[color] + color * 16;
            this->slideMovement(fig, getFigureNbFromPosIdx(fig, color));
        }
        this->attack[color] = false;
        nextPos = this->next_pos[color];
    }
    int8 attacked_fig = getPositionIndex(board.board[nextPos], (color + 1) % 2);
	int8 number = 1;
    while (board.positions[attacked_fig] != nextPos && number <= 8){
        attacked_fig = getPositionIndex(board.board[nextPos], (color + 1) % 2, number);
		number++;
	}
	#ifdef SAFEMODE
    if (board.positions[attacked_fig] != nextPos)
		printError("attack fig not found");
	#endif
	board.positions[attacked_fig] = DESTROYED;
    
}

void Generator::staticDestroyAttackedFig(Board &board, int8 pos, int8 color, int8 next_pos){
    int8 attacked_fig = getPositionIndex(board.board[next_pos], not(color));
    if (board.board[next_pos] == K)
        board.states.shah = not(color);
    int8 number = 1;
    while (board.positions[attacked_fig] != next_pos && number < 8){
        attacked_fig = getPositionIndex(board.board[next_pos], not(color), number);
        number++;
    }
    assert(board.positions[attacked_fig] == next_pos);
    board.positions[attacked_fig] = DESTROYED;
}

void Generator::slideMovement(int8 fig, int8 fig_move_idx, bool increase_range){
	int8 color = fig / 16;
	if (increase_range && *moves[getFigureNbFromPosIdx(fig,color)].rbegin() == RANGED_FAR && this->current_move_distance[color] < 8){
		this->current_move_distance[color]++;
		return;
	}
	this->current_move_distance[color] = 1;
	if (this->current_move[color] < (int8)moves[fig_move_idx].size() - 1){
		this->current_move[color]++;
		return;
	}
	this->current_move[color] = 0;
	this->current_figure[color]++;
}

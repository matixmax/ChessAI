
#include "Valuator.h"
#include "Board.h"
#include "FigInfo.h"
#include "MathLogicStore.h"

using namespace std;

namespace Valuation
{
	
Valuator& Valuator::i()
{
	static Valuator instance;
	return instance;
}
	
Valuator::Valuator()
{
	m_figValues = { 20000,  //king
	                950,    //queen
	                500,    //rook
	                290,    //knight
	                310,    //bishop
	                100,    //black pawn
	                100 };  //white pawn
}
	
//return pair white material and black material
pair<int, int> Valuator::countMaterial(const Board & chessboard) {
	int material_sum = 0;
	int white_material = 0;
	int black_material;
	for (int i = 0; i < BOARD_SIZE; i++) {
	    if (chessboard.board[i] != EMPTY && chessboard.board[i] != K) {
	        material_sum += m_figValues[chessboard.board[i]];
	        if (chessboard.colors[i] == WHITE)
	            white_material += m_figValues[chessboard.board[i]];
	    }
	}
	black_material = material_sum - white_material;
	return make_pair(white_material, black_material);
}
	
Valuation::GameState Valuator::getGamePhase(const Board & chessboard)
{
	pair<int, int> material = countMaterial(chessboard);
	int black_material = material.second;
	int white_material = material.first;
	if (getFiguresInStartPos(chessboard) >= 14)
	    return DEBUT;
	if (black_material - white_material >= 450 && matCondition(chessboard, BLACK))
	    return MATTING;
	if (white_material - black_material >= 450 && matCondition(chessboard, WHITE))
	    return MATTING;
	if (white_material + black_material > 4300)
	    return MID_GAME;
	if (white_material + black_material > 2800)
	    return EARLY_ENDING;
	return ENDING;
}
	
int Valuator::materialValuation(const Board & chessboard, int color) {
	int better_side_color;
	int material_advantage;
	
	pair<int, int> material = countMaterial(chessboard);
	if (material.first == material.second)
	    return 0;
	if (material.first > material.second) {
	    better_side_color = WHITE;
	    material_advantage = material.first - material.second;
	}
	else {
	    better_side_color = BLACK;
	    material_advantage = material.second - material.first;
	}
	
	int betterSidePawnsSum = countSumOfPawns(chessboard, better_side_color);
	int pawns_material = MathLogicStore::getValueOfPawns(betterSidePawnsSum, material);
	if (color == better_side_color)
	    return MathLogicStore::getValueOfMaterial(pawns_material, material_advantage, material);
	else
	    return -MathLogicStore::getValueOfMaterial(pawns_material, material_advantage, material);
}
	
//int Valuator::queenPositionalValue(const Board & chessboard, int color, int phase) {
//	int value = 0;
//	int factor = 0;
//	int my_queen = chessboard.positions[FigInfo::getPosIndex(H, color)];
//	int enemy_queen = chessboard.positions[FigInfo::getPosIndex(H, FigInfo::not(color))];
//	int my_king = chessboard.positions[FigInfo::getPosIndex(K, color)];
//	int enemy_king = chessboard.positions[FigInfo::getPosIndex(K, FigInfo::not(color))];
//	switch (phase) {
//	case DEBUT:
//	    factor = -2;
//	    break;
//	case MID_GAME:
//	    factor = 0;
//	    break;
//	case EARLY_ENDING:
//	    factor = 2;
//	    break;
//	case ENDING:
//	    factor = 4;
//	    break;
//	}
//	value += factor * proximityToTheCenter(my_queen);
//	value -= factor * proximityToTheCenter(enemy_queen);
//	if (phase != DEBUT) {
//	    value += 3 * proximityOfFields(my_queen, enemy_king);
//	    value -= 3 * proximityOfFields(enemy_queen, my_king);
//	}
//	return value;
//}
//	
int Valuator::kingPositionalValue(const Board & chessboard, int color, int phase) {
	int value = 0;
	int factor = 0;
	switch (phase) {
	case DEBUT:
	case MID_GAME:
	    value += safetyKingPossitionalValue(chessboard, color);
	    break;
	case ENDING:
	    factor += 5;
	case EARLY_ENDING:
	    factor += 3;
	    value += factor * proximityToTheCenter(chessboard.positions[FigInfo::getPosIndex(K, color)]);
	    value -= factor * proximityToTheCenter(chessboard.positions[FigInfo::getPosIndex(K, FigInfo::not(color))]);
	    break;
	}
	return value;
}
	
int Valuator::knightsPositionalValue(const Board & chessboard, int color) {
	int valuate = 0;
	valuate += proximityToTheCenter(chessboard.positions[FigInfo::getPosIndex(S, color, 0)]);
	valuate += proximityToTheCenter(chessboard.positions[FigInfo::getPosIndex(S, color, 1)]);
	valuate -= proximityToTheCenter(
	    chessboard.positions[FigInfo::getPosIndex(S, FigInfo::not(color), 0)]);
	valuate -= proximityToTheCenter(
	    chessboard.positions[FigInfo::getPosIndex(S, FigInfo::not(color), 1)]);
	return valuate;
}
	
int Valuator::bishopsPositionalValue(const Board & chessboard, int color, int phase) {
	int bishop_movements[] = { 7,  7,  7,  7,  7,  7,  7,  7,
	    7,  9,  9,  9,  9,  9,  9,  7,
	    7,  9, 11, 11, 11, 11,  9,  7,
	    7,  9, 11, 13, 13, 11,  9,  7,
	    7,  9, 11, 13, 13, 11,  9,  7,
	    7,  9, 11, 11, 11, 11,  9,  7,
	    7,  9,  9,  9,  9,  9,  9,  7,
	    7,  7,  7,  7,  7,  7,  7,  7 };
	int points_for_black[] = { 0, 3, 2, 0, 1, 0, 1, 0, -1, 0, 0, 0, 0, 0, 0, 0,
	    10, 9, 5, 0, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int points_for_white[] = { 10, 9, 5, 0, 3, 0, 4, 0, -1, 0, 0, 0, 0, 0, 0, 0,
	    0, 3, 2, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//points for the presence of the pieces, my pieces in begin look down for figures_in_diagonals
	int values[] = { 0, 0, 0, 0 };
	int bishop_colors[] = { WHITE, WHITE, BLACK, BLACK };
	for (int i = 0; i < 4; i++) {
	    int my_pos = chessboard.positions[FigInfo::getPosIndex(G, bishop_colors[i], i % 2)];
	    if (my_pos == DESTROYED)
	        continue;
	    int figures_in_diagonals[] = { -1, -1, -1, -1 };//diagonals in accordance with the clockwise
	    int max_distance = 7 - my_pos % ROW_SIZE; // to the right
	    for (int distance = 1; distance <= max_distance; distance++) {
	        if (figures_in_diagonals[0] == -1 && my_pos + (-7 * distance) > 0 && chessboard.board[my_pos + (-7 * distance)] != EMPTY)
	            figures_in_diagonals[0] = FigInfo::getPosIndex(chessboard.board[my_pos + (-7 * distance)], chessboard.colors[my_pos + (-7 * distance)]);
	        if (figures_in_diagonals[1] == -1 && my_pos + (9 * distance) < BOARD_SIZE && chessboard.board[my_pos + (9 * distance)] != EMPTY)
	            figures_in_diagonals[1] = FigInfo::getPosIndex(chessboard.board[my_pos + (9 * distance)], chessboard.colors[my_pos + (9 * distance)]);
	    }
	    max_distance = my_pos % ROW_SIZE; // to the left
	    for (int distance = 1; distance <= max_distance; distance++) {
	        if (figures_in_diagonals[2] == -1 && my_pos + (7 * distance) < BOARD_SIZE && chessboard.board[my_pos + (7 * distance)] != EMPTY)
	            figures_in_diagonals[2] = FigInfo::getPosIndex(chessboard.board[my_pos + (7 * distance)], chessboard.colors[my_pos + (7 * distance)]);
	        if (figures_in_diagonals[3] == -1 && my_pos + (-9 * distance) > 0 && chessboard.board[my_pos + (-9 * distance)] != EMPTY)
	            figures_in_diagonals[3] = FigInfo::getPosIndex(chessboard.board[my_pos + (-9 * distance)], chessboard.colors[my_pos + (-9 * distance)]);
	    }
	    for (int j = 0; j < 4; j++) {
	        if (figures_in_diagonals[j] != -1) {
	            if (i < 2)
	                values[i] += points_for_white[figures_in_diagonals[j]];
	            else
	                values[i] += points_for_black[figures_in_diagonals[j]];
	        }
	    }
	    //TODO: should check pawns in front read more http://www.apsys.waw.pl/joanna2002/pracmag.pdf page 44
	    values[i] += 7 - bishop_movements[my_pos];
	}
	if (color == WHITE)return values[0] + values[1] - values[2] - values[3];
	return values[2] + values[3] - values[0] - values[1];
}
	
int Valuator::mattingPositionalValue(const Board & chessboard, int color) {//color = matting page color
	int value = 0;
	int enemy_king = chessboard.positions[FigInfo::getPosIndex(K, FigInfo::not(color))];
	value -= 9 * proximityToTheCenter(enemy_king);
	int my_knights[2] = { chessboard.positions[FigInfo::getPosIndex(S, color, 0)], chessboard.positions[FigInfo::getPosIndex(S, color, 1)] };
	int my_king = chessboard.positions[FigInfo::getPosIndex(K, color)];
	value += 2 * proximityOfFields(my_knights[0], enemy_king);
	value += 2 * proximityOfFields(my_knights[1], enemy_king);
	value += 2 * proximityOfFields(my_king, enemy_king);
	if (my_king / ROW_SIZE == 2 || my_king / ROW_SIZE == 5 || my_king % ROW_SIZE == 2 || my_king % ROW_SIZE == 5)
	    value += 2;
	return value;
}
	
int Valuator::safetyKingPossitionalValue(const Board & chessboard, int color) {
    int king_safety[] = {    15,   25,   10,  -5,   -10,   5,    25,   15,
                             10,   15,  -40,  -40,  -40,  -40,   15,   10,
                            -100, -100, -100, -100, -100, -100, -100, -100,
                            -200, -200, -200, -200, -200, -200, -200, -200,
                            -200, -200, -200, -200, -200, -200, -200, -200,
                            -100, -100, -100, -100, -100, -100, -100, -100,
                             10,   15,  -40,  -40,  -40,  -40,   15,   10,
                             15,   25,   10,  -5,   -10,   5,    25,   15 };
	int value = 0;
	int my_king = chessboard.positions[FigInfo::getPosIndex(K, color)];
	int enemy_king = chessboard.positions[FigInfo::getPosIndex(K, FigInfo::not(color))];
    if (my_king == DESTROYED || enemy_king == DESTROYED)
        return 0;
	if (color == WHITE) {
	    (my_king / ROW_SIZE < 4) ? value -= 400 : value += king_safety[my_king];
	    (enemy_king / ROW_SIZE >= 4) ? value += 400 : value -= king_safety[enemy_king];
	}
	else {
	    (my_king / ROW_SIZE >= 4) ? value -= 400 : value += king_safety[my_king];
	    (enemy_king / ROW_SIZE < 4) ? value += 400 : value -= king_safety[enemy_king];
	}
	int kings_pos[] = { my_king, enemy_king };
	value += wallOfPawnsValuation(chessboard, color, kings_pos);
	return value;
}
	
int Valuator::wallOfPawnsValuation(const Board & chessboard, int color, int *kings_pos) {
	int value[2] = { 0, 0 };
	int color_direction[] = { 1, -1 };
	int last_line[] = { 7, 0 };
	for (int i = 0; i < 2; i++) {
	    if (kings_pos[i] / ROW_SIZE != last_line[color]) {
	        if (chessboard.board[kings_pos[i] + ROW_SIZE * color_direction[color]] == P + color)
	            value[i] += 15;
	        if (kings_pos[i] % ROW_SIZE != 0 && chessboard.board[kings_pos[i] + ROW_SIZE * color_direction[color] - 1] == P + color)
	            value[i] += 15;
	        if (kings_pos[i] % ROW_SIZE != 7 && chessboard.board[kings_pos[i] + ROW_SIZE * color_direction[color] + 1] == P + color)
	            value[i] += 15;
	        if (value[i] == 45)value[i] = 0;
	    }
	}
	return value[0] - value[1];
}
	
//int Valuator::connectedRooksAndMobilityValue(const Board & chessboard, int* rooks_pos, int* rooks_color) {
//	int values[] = { 0, 0, 0, 0 };
//	for (int i = 0; i < 4; i++) {
//	    if (rooks_pos[i] == DESTROYED)
//	        continue;
//	    int figures_in_lines[] = { -1, -1, -1, -1 };
//	    int move[] = { 0, -1, 1, 0, 0, 1, -1, 0 };
//	    int movement_counter = 0;
//	    int pos_x = rooks_pos[i] % ROW_SIZE, pos_y = rooks_pos[i] / ROW_SIZE;
//	    for (int j = 0; j < 4; j++) {
//	        int pos[2] = { pos_x + move[2 * j], pos_y + move[2 * j + 1] };
//	        while (pos[0] >= 0 && pos[1] >= 0 && pos[0] <= 7 && pos[1] <= 7) {
//	            if (chessboard.board[pos[1] * ROW_SIZE + pos[0]] != EMPTY) {
//	                figures_in_lines[j] = FigInfo::getPosIndex(chessboard.board[pos[1] * ROW_SIZE + pos[0]], chessboard.colors[pos[1] * ROW_SIZE + pos[0]]);
//	                break;
//	            }
//	            movement_counter++;
//	            pos[0] += move[2 * j];
//	            pos[1] += move[2 * j + 1];
//	        }
//	        if (figures_in_lines[j] != -1) {
//	            if (figures_in_lines[j] / 16 == FigInfo::not(rooks_color[i]))
//	                movement_counter++;
//	            else {
//	                if (FigInfo::getFigNumber(figures_in_lines[j], rooks_color[i]) == W)
//	                    values[i] += 10;
//	            }
//	        }
//	        values[i] += movement_counter;
//	    }
//	}
//	return values[0] + values[1] - values[2] - values[3];
//}
		
int Valuator::proximityToTheCenter(int pos) {
	if (pos == DESTROYED)
	    return 0;
	int x = pos / ROW_SIZE;
	int y = pos % ROW_SIZE;
	return 7 - (int)(abs(3.5 - (float)x) + abs(3.5 - (float)y));
}
	
int Valuator::proximityOfFields(int pos1, int pos2) {
	if (pos1 == DESTROYED || pos2 == DESTROYED)
	    return 0;
	int x1 = pos1 / ROW_SIZE, x2 = pos2 / ROW_SIZE;
	int y1 = pos1 % ROW_SIZE, y2 = pos2 % ROW_SIZE;
	return 7 - (abs(x1 - x2) + abs(y1 - y2));
}
	
bool Valuator::matCondition(const Board &chessboard, int color) {
	if (chessboard.positions[FigInfo::getPosIndex(H, color)] != DESTROYED)
	    return true;
	if (chessboard.positions[FigInfo::getPosIndex(W, color, 0)] != DESTROYED
	    || chessboard.positions[FigInfo::getPosIndex(W, color, 1)] != DESTROYED)
	    return true;
	int sum_of_pawns = countSumOfPawns(chessboard, color);
	if (sum_of_pawns == 0)
	    return true;
	return false;
}
	
int Valuator::getFiguresInStartPos(const Board & chessboard) {
	int figures_in_start = 0;
	int begin_positions[NUMBER_OF_POSITIONS] =
	    {   4,  3,   0,  7,  1,  6,  2,  5,  //    position K H W W S S G G
	        8,  9,  10, 11, 12, 13, 14, 15,  //             PbPbPbPbPbPbPbPb
	        60, 59, 56, 63, 57, 62, 58, 61,  //             K H W W S S G G
	        48, 49, 50, 51, 52, 53, 54, 55 };//             PwPwPwPbwbPwPwPw
	for (int i = 0; i < NUMBER_OF_POSITIONS; i++) {
	    if (chessboard.positions[i] == begin_positions[i] && i != FigInfo::getPosIndex(K, i / 16))
	        figures_in_start++;
	}
	return figures_in_start;
}
	
int Valuator::countSumOfPawns(const Board & chessboard, int color) {
	int sum_of_pawns = 0;
	for (int i = 0; i < 8; i++) {
	    if (chessboard.positions[FigInfo::getPosIndex(P, color, i)] != DESTROYED)
	        sum_of_pawns++;
	}
	return sum_of_pawns;
}
}
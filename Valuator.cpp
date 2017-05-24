
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
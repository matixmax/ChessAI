#include "ValuationFunctions.h"
#include "Board.h"

using namespace std;

int *figure_values = new int[7];

void setValues(){
    int tmp[]={ 20000,  //king
                950,    //queen
                500,    //rook
                290,    //knight
                310,    //bishop
                100,    //balck pawn
                100};   //white pawn
    memcpy(figure_values, tmp, 7 * sizeof(int));
}

bool matCondition(const Board &chessboard, int8 color){
    if (chessboard.positions[getPositionIndex(H, color)] != DESTROYED)
        return true;
    if (chessboard.positions[getPositionIndex(W, color, 0)] != DESTROYED
        || chessboard.positions[getPositionIndex(W, color, 1)] != DESTROYED)
        return true;
    int sum_of_pawns = countSumOfPawns(chessboard, color);
    if (sum_of_pawns == 0)
        return true;
    return false;
}

int getFiguresInStartPos(const Board & chessboard){
    int figures_in_start = 0;
    int8 begin_positions[32] =
    { 4, 3, 0, 7, 1, 6, 2, 5,        // position K H W W S S G G
    8, 9, 10, 11, 12, 13, 14, 15,    //			 PbPbPbPbPbPbPbPb
    60, 59, 56, 63, 57, 62, 58, 61,  //			 K H W W S S G G
    48, 49, 50, 51, 52, 53, 54, 55 };//			 PwPwPwPbwbPwPwPw
    for (int i = 0; i < NUMBER_OF_POSITIONS; i++){
        if (chessboard.positions[i] == begin_positions[i] && i != getPositionIndex(K, i/16))
            figures_in_start++;
    }
    return figures_in_start;
}

int countSumOfPawns(const Board & chessboard, int8 color){
    int sum_of_pawns = 0;
    for (int i = 0; i < 8; i++){
        if (chessboard.positions[getPositionIndex(P, color, i)] != DESTROYED)
            sum_of_pawns++;
    }
    return sum_of_pawns;
}

//return pair white material and black material
pair<int, int> countMaterial(const Board & chessboard){
    int material_sum = 0;
    int white_material = 0;
    int black_material;
    for (int i = 0; i < 64; i++){
        if (chessboard.board[i] != EMPTY && chessboard.board[i] != K){
            material_sum += figure_values[chessboard.board[i]];
            if (chessboard.colors[i] == WHITE)
                white_material += figure_values[chessboard.board[i]];
        }
    }
    black_material = material_sum - white_material;
    return make_pair(white_material, black_material);
}

//return five phase: debut= 0, mid game= 1, early ending= 2, ending= 3, matting= 4
int checkGamePhase(const Board & chessboard){
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

int materialValuation(const Board & chessboard, int8 color){
    int8 better_side_color;
    int material_advantage;
    pair<int,int> material = countMaterial(chessboard);
    if (material.first == material.second)
        return 0;
    if (material.first > material.second){
        better_side_color = WHITE;
        material_advantage = material.first - material.second;
    }
    else{
        better_side_color = BLACK;
        material_advantage = material.second - material.first;
    }
    int pawns_sum = countSumOfPawns(chessboard, better_side_color);
    int pawns_material = pawns_sum * 30000 / ((pawns_sum + 1) * (material.first + material.second));
    if(color == better_side_color) 
        return pawns_material + (material_advantage * 590 * 2 / (material.first + material.second));
    else
        return -(pawns_material + (material_advantage * 590 * 2 / (material.first + material.second)));
}

int proximityToTheCenter(int8 pos){
    if (pos == DESTROYED)
        return 0;
    int8 x = pos / 8;
    int8 y = pos % 8;
    return 7 - (int)(abs(3.5 - (float)x) + abs(3.5 - (float)y));
}

int proximityOfFields(int8 pos1 ,int8 pos2){
    if (pos1 == DESTROYED || pos2 == DESTROYED)
        return 0;
    int8 x1 = pos1 / 8, x2 = pos2 / 8;
    int8 y1 = pos1 % 8, y2 = pos2 % 8;
    return 7 - (abs(x1 - x2) + abs(y1 - y2));
}

bool izolatedPawn(const Board & chessboard, int8 color, int8 pos){
    int8 neighborhood[2];
    int8 my_fig;
    if (color == WHITE){ my_fig = Pb; neighborhood[0] = 9; neighborhood[1] = 7; }
    else{ my_fig = Pc; neighborhood[0] = -7; neighborhood[1] = -9; }
    if (pos % 8 != 0 && chessboard.board[pos + neighborhood[1]] == my_fig)
        return false;
    if (pos % 8 != 7 && chessboard.board[pos + neighborhood[0]] == my_fig)
        return false;
    return true;
}

bool mirroredPawn(const Board & chessboard, int8 color, int8 pos){
    int y = pos % 8;
    int8 my_fig = P + color;
    for (int i = 0; i < 8; i++){
        if (chessboard.board[y + i * 8] == my_fig && y + i * 8 != pos)
            return true;
    }
    return false;
}

int pawnsPositionalValue(const Board & chessboard, int8 color, int phase){
    int value = 0;
    for (int pawn = 0; pawn < 8; pawn++){
        int my_pos = chessboard.positions[getPositionIndex(P, color, pawn)];
        int enemy_pos = chessboard.positions[getPositionIndex(P, not(color), pawn)];
        if (my_pos != DESTROYED && izolatedPawn(chessboard, color, my_pos))
            value -= 20;
        if (enemy_pos != DESTROYED && izolatedPawn(chessboard, not(color), enemy_pos))
            value += 20;
        if (my_pos != DESTROYED && mirroredPawn(chessboard, color, my_pos))
            value -= 10;
        if (phase == DEBUT || phase == MID_GAME){
            int proximity = proximityToTheCenter(my_pos);
            if (proximity == 6)
                value += 15;
            if (proximity == 5)
                value += 10;
            value += proximity;
        }
        if (phase == EARLY_ENDING || phase == ENDING){
            int factor = 2;
            if (phase == ENDING)
                factor += 2;
            for (int i = 8; i < 16; i++){
                int pos_x = chessboard.positions[i]/8;
                int enemy_pos_x = chessboard.positions[i + 16]/8;
                if (color == WHITE){
                    swap(pos_x, enemy_pos_x);
                    if (chessboard.positions[i] != DESTROYED)value += factor * (6 - pos_x) * (6 - pos_x);
                    if (chessboard.positions[i + 16] != DESTROYED)value -= factor * (enemy_pos_x - 1) * (enemy_pos_x - 1);
                }
                else{
                    if (chessboard.positions[i] != DESTROYED)value += factor * (pos_x - 1) * (pos_x - 1);
                    if (chessboard.positions[i + 16] != DESTROYED)value -= factor * (6 - enemy_pos_x) * (6 - enemy_pos_x);
                }
                // here should by check pawns promotion but this is no supported now
            }
        }
    }
    return value;
}

int knightsPositionalValue(const Board & chessboard, int8 color){
    int valuate = 0;
    valuate += proximityToTheCenter(chessboard.positions[getPositionIndex(S, color, 0)]);
    valuate += proximityToTheCenter(chessboard.positions[getPositionIndex(S, color, 1)]);
    valuate -= proximityToTheCenter(chessboard.positions[getPositionIndex(S, not(color), 0)]);
    valuate -= proximityToTheCenter(chessboard.positions[getPositionIndex(S, not(color), 1)]);
    return valuate;
}

int bishopsPositionalValue(const Board & chessboard, int8 color, int phase){
    int bishop_movements[] = {  7,  7,  7,  7,  7,  7,  7,  7,
                                7,  9,  9,  9,  9,  9,  9,  7,
                                7,  9, 11, 11, 11, 11,  9,  7,
                                7,  9, 11, 13, 13, 11,  9,  7,
                                7,  9, 11, 13, 13, 11,  9,  7,
                                7,  9, 11, 11, 11, 11,  9,  7,
                                7,  9,  9,  9,  9,  9,  9,  7,
                                7,  7,  7,  7,  7,  7,  7,  7 };
    int points_for_black[] = { 0, 3, 2, 0, 1, 0, 1, 0, -1, 0, 0, 0, 0, 0, 0, 0,
                              10, 9, 5, 0, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int points_for_white[] = { 10, 9, 5, 0, 3, 0, 4, 0, -1, 0, 0, 0, 0, 0, 0, 0, 
                                0, 3, 2, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //points for the presence of the pieces, my pieces in begin look down for figures_in_diagonals
    int values[] = { 0, 0, 0, 0 };
    int8 bishop_colors[] = { WHITE, WHITE, BLACK, BLACK };
    for (int i = 0; i < 4; i++){
        int8 my_pos = chessboard.positions[getPositionIndex(G, bishop_colors[i], i % 2)];
        if (my_pos == DESTROYED)
            continue;
        int8 figures_in_diagonals[] = { -1, -1, -1, -1 };//diagonals in accordance with the clockwise
        int max_distance = 7 - my_pos % 8; // to the right
        for (int distance = 1; distance <= max_distance; distance++){
            if (figures_in_diagonals[0] == -1 && my_pos + (-7 * distance) > 0 && chessboard.board[my_pos + (-7 * distance)] != EMPTY)
                figures_in_diagonals[0] = getPositionIndex(chessboard.board[my_pos + (-7 * distance)], chessboard.colors[my_pos + (-7 * distance)]);
            if (figures_in_diagonals[1] == -1 && my_pos + (9 * distance) < 64 && chessboard.board[my_pos + (9 * distance)] != EMPTY)
                figures_in_diagonals[1] = getPositionIndex(chessboard.board[my_pos + (9 * distance)], chessboard.colors[my_pos + (9 * distance)]);
        }
        max_distance = my_pos % 8; // to the left
        for (int distance = 1; distance <= max_distance; distance++){
            if (figures_in_diagonals[2] == -1 && my_pos + (7 * distance) < 64 && chessboard.board[my_pos + (7 * distance)] != EMPTY)
                figures_in_diagonals[2] = getPositionIndex(chessboard.board[my_pos + (7 * distance)], chessboard.colors[my_pos + (7 * distance)]);
            if (figures_in_diagonals[3] == -1 && my_pos + (-9 * distance) > 0 && chessboard.board[my_pos + (-9 * distance)] != EMPTY)
                figures_in_diagonals[3] = getPositionIndex(chessboard.board[my_pos + (-9 * distance)], chessboard.colors[my_pos + (-9 * distance)]);
        }
        for (int j = 0; j < 4; j++){
            if (figures_in_diagonals[j] != -1){
                if(i < 2)
                    values[i] += points_for_white[figures_in_diagonals[j]];
                else
                    values[i] += points_for_black[figures_in_diagonals[j]];
            }
        }
        //should check pawns in front read more http://www.apsys.waw.pl/joanna2002/pracmag.pdf page 44
        values[i] += 7 - bishop_movements[my_pos];
    }
    if(color == WHITE)return values[0] + values[1] - values[2] - values[3];
    return values[2] + values[3] - values[0] - values[1];
}

int rookInOpenLineValue(const Board & chessboard, int8* rooks_pos, int8* rooks_color){
    int value[] = { 0, 0, 0, 0 };
    for (int i = 0; i < 4; i++){
        if (rooks_pos[i] == DESTROYED)
            continue;
        int8 pos_x = rooks_pos[i] % 8;
        int8 my_pawn = P + rooks_color[i], enemy_pawn = P + not(rooks_color[i]);
        bool open = true, self_open = true;
        for (int j = 0; j < 8; j++){
            if (chessboard.board[pos_x + j * 8] == my_pawn)
                self_open = false;
            if (chessboard.board[pos_x + j * 8] == enemy_pawn)
                open = false;
        }
        if (self_open){
            value[i] += 4;
            if (open)
                value[i] += 6;
        }
    }
    return value[0] + value[1] - value[2] - value[3];
}

int connectedRooksAndMobilityValue(const Board & chessboard, int8* rooks_pos, int8* rooks_color){
    int values[] = { 0, 0, 0, 0 };
    for (int i = 0; i < 4; i++){
        if (rooks_pos[i] == DESTROYED)
            continue;
        int8 figures_in_lines[] = { -1, -1, -1, -1 };
        int move[] = { 0, -1, 1, 0, 0, 1, -1, 0 };
        int movement_counter = 0;
        int8 pos_x = rooks_pos[i] % 8, pos_y = rooks_pos[i] / 8;
        for (int j = 0; j < 4; j++){
            int pos[2] = { pos_x + move[2 * j], pos_y + move[2 * j + 1] };
            while (pos[0] >= 0 && pos[1] >= 0 && pos[0] <= 7 && pos[1] <= 7){
                if (chessboard.board[pos[1] * 8 + pos[0]] != EMPTY){
                    figures_in_lines[j] = getPositionIndex(chessboard.board[pos[1] * 8 + pos[0]], chessboard.colors[pos[1] * 8 + pos[0]]);
                    break;
                }
                movement_counter++;
                pos[0] += move[2 * j]; 
                pos[1] += move[2 * j + 1];
            }
            if (figures_in_lines[j] != -1){
                if (figures_in_lines[j] / 16 == not(rooks_color[i]))
                    movement_counter++;
                else{
                    if (getFigureNbFromPosIdx(figures_in_lines[j], rooks_color[i]) == W)
                        values[i] += 10;
                }
            }
            values[i] += movement_counter;
        }
    }
    return values[0] + values[1] - values[2] - values[3];
}

int rooksPositionalValue(const Board & chessboard, int8 color, int phase){
    int value = 0;
    int8 rooks_pos[4];
    int8 rooks_color[] = { color, color, not(color), not(color) };
    int8 king_pos[2];
    if (color == WHITE){
        if (chessboard.board[56] == W && chessboard.colors[56] == color)value += 5;
        if (chessboard.board[63] == W && chessboard.colors[63] == color)value += 5;
    }
    else{
        if (chessboard.board[0] == W && chessboard.colors[0] == color)value += 5;
        if (chessboard.board[7] == W && chessboard.colors[7] == color)value += 5;
    }
    for (int i = 0; i < 4; i++)
        rooks_pos[i] = chessboard.positions[getPositionIndex(W, rooks_color[i], i % 2)];
    king_pos[0] = chessboard.positions[getPositionIndex(K, color)];
    king_pos[1] = chessboard.positions[getPositionIndex(K, not(color))];
    value += rookInOpenLineValue(chessboard, rooks_pos, rooks_color);
    if (phase != DEBUT){
        value += 2 * proximityOfFields(rooks_pos[0], king_pos[1]);
        value += 2 * proximityOfFields(rooks_pos[1], king_pos[1]);
        value -= 2 * proximityOfFields(rooks_pos[2], king_pos[0]);
        value -= 2 * proximityOfFields(rooks_pos[3], king_pos[0]);
    }
    value += connectedRooksAndMobilityValue(chessboard, rooks_pos, rooks_color);
    int8 seventh_line[] = { 6, 1 }, eigth_line[] = { 7, 0 };
    for (int i = 0; i < 4; i++){
        if (rooks_pos[i] / 8 == seventh_line[rooks_color[i]]){ //if destroyed this condition is false
            (i < 2) ? value += 27 : value -= 27;
            if (king_pos[rooks_color[i]] / 8 == eigth_line[rooks_color[i]])
                (i < 2) ? value += 13 : value -= 13;
        }
    }
    return value;
}

int queenPositionalValue(const Board & chessboard, int8 color, int phase){
    int value = 0;
    int factor = 0;
    int my_queen = chessboard.positions[getPositionIndex(H, color)];
    int enemy_queen = chessboard.positions[getPositionIndex(H, not(color))];
    int my_king = chessboard.positions[getPositionIndex(K, color)];
    int enemy_king = chessboard.positions[getPositionIndex(K, not(color))];
    switch (phase){
    case DEBUT:
        factor = -2;
        break;
    case MID_GAME:
        factor = 0;
        break;
    case EARLY_ENDING:
        factor = 2;
        break;
    case ENDING:
        factor = 4;
        break;
    }
    value += factor * proximityToTheCenter(my_queen);
    value -= factor * proximityToTheCenter(enemy_queen);
    if (phase != DEBUT){
        value += 3 * proximityOfFields(my_queen, enemy_king);
        value -= 3 * proximityOfFields(enemy_queen, my_king);
    }
    return value;
}

int wallOfPawnsValuation(const Board & chessboard, int8 color, int8 *kings_pos){
    int value[2] = { 0, 0 };
    int8 color_direction[] = { 1, -1 };
    int8 last_line[] = { 7, 0 };
    for (int i = 0; i < 2; i++){
        if (kings_pos[i] / 8 != last_line[color]){
            if (chessboard.board[kings_pos[i] + 8 * color_direction[color]] == P + color)
                value[i] += 15;
            if (kings_pos[i] % 8 != 0 && chessboard.board[kings_pos[i] + 8 * color_direction[color] - 1] == P + color)
                value[i] += 15;
            if (kings_pos[i] % 8 != 7 && chessboard.board[kings_pos[i] + 8 * color_direction[color] + 1] == P + color)
                value[i] += 15;
            if (value[i] == 45)value[i] = 0;
        }
    }
    return value[0] - value[1];
}

int safetyKingPossitionalValue(const Board & chessboard, int8 color){
    int king_safety[] = { 15, 25, 10, -5, -10, 5, 25, 15,
        10, 15, -40, -40, -40, -40, 15, 10,
        -100, -100, -100, -100, -100, -100, -100, -100,
        -200, -200, -200, -200, -200, -200, -200, -200,
        -200, -200, -200, -200, -200, -200, -200, -200,
        -100, -100, -100, -100, -100, -100, -100, -100,
        10, 15, -40, -40, -40, -40, 15, 10,
        15, 25, 10, -5, -10, 5, 25, 15 };
    int value = 0;
    int8 my_king = chessboard.positions[getPositionIndex(K, color)];
    int8 enemy_king = chessboard.positions[getPositionIndex(K, not(color))];
    if (color == WHITE){
        (my_king / 8 < 4) ? value -= 400 : value += king_safety[my_king];
        (enemy_king / 8 >= 4) ? value += 400 : value -= king_safety[enemy_king];
    }
    else{
        (my_king / 8 >= 4) ? value -= 400 : value += king_safety[my_king];
        (enemy_king / 8 < 4) ? value += 400 : value -= king_safety[enemy_king];
    }
    int8 kings_pos[] = { my_king, enemy_king };
    value += wallOfPawnsValuation(chessboard, color, kings_pos);
    return value;
}

int kingPositionalValue(const Board & chessboard, int8 color, int phase){
    int value = 0;
    int factor = 0;
    switch (phase){
    case DEBUT:
    case MID_GAME:
        value += safetyKingPossitionalValue(chessboard, color);
        break;
    case ENDING:
        factor += 5;
    case EARLY_ENDING:
        factor += 3;
        value += factor * proximityToTheCenter(chessboard.positions[getPositionIndex(K, color)]);
        value -= factor * proximityToTheCenter(chessboard.positions[getPositionIndex(K, not(color))]);
        break;
    }
    return value;
}

int mattingPositionalValue(const Board & chessboard, int8 color){//color = matting page kolor
    int value = 0;
    int8 enemy_king = chessboard.positions[getPositionIndex(K, not(color))];
    value -= 9 * proximityToTheCenter(enemy_king);
    int8 my_knights[2] = { chessboard.positions[getPositionIndex(S, color, 0)], chessboard.positions[getPositionIndex(S, color, 1)] };
    int8 my_king = chessboard.positions[getPositionIndex(K, color)];
    value += 2 * proximityOfFields(my_knights[0], enemy_king);
    value += 2 * proximityOfFields(my_knights[1], enemy_king);
    value += 2 * proximityOfFields(my_king, enemy_king);
    if (my_king / 8 == 2 || my_king / 8 == 5 || my_king % 8 == 2 || my_king % 8 == 5)
        value += 2;
    return value;
}
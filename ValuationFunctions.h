#pragma once

#include "Definitions.h"
#include "PreparatoryFunctions.h"
#include "Board.h"

#define DEBUT 0
#define MID_GAME 1
#define EARLY_ENDING 2
#define ENDING 3
#define MATTING 4

extern int *figure_values;

void setValues();
bool matCondition(const Board & chessboard, int8 color);
int getFiguresInStartPos(const Board & chessboard);
int countSumOfPawns(const Board & chessboard, int8 color);
std::pair<int, int> countMaterial(const Board & chessboard);
int checkGamePhase(const Board & chessboard);
int materialValuation(const Board & chessboard, int8 color);
int proximityToTheCenter(int8 pos);
int proximityOfFields(int8 pos1, int8 pos2);
bool izolatedPawn(const Board & chessboard, int8 color, int8 pos);
bool mirroredPawn(const Board & chessboard, int8 color, int8 pos);
int pawnsPositionalValue(const Board & chessboard, int8 color, int phase);
int knightsPositionalValue(const Board & chessboard, int8 color);
int bishopsPositionalValue(const Board & chessboard, int8 color, int phase);
int rookInOpenLineValue(const Board & chessboard, int8* rooks_pos, int8* rooks_color);
int connectedRooksAndMobilityValue(const Board & chessboard, int8* rooks_pos, int8* rooks_color);
int rooksPositionalValue(const Board & chessboard, int8 color, int phase);
int queenPositionalValue(const Board & chessboard, int8 color, int phase);
int wallOfPawnsValuation(const Board & chessboard, int8 color, int8 *kings_pos);
int safetyKingPossitionalValue(const Board & chessboard, int8 color);
int kingPositionalValue(const Board & chessboard, int8 color, int phase);
int mattingPositionalValue(const Board & chessboard, int8 color);
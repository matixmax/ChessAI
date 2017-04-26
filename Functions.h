#pragma once
#include "Definitions.h"

//Preparatory Functions
void setInitialBoard();
void setMoves();
void printBoard();
void printBoard(const Board &chessboard);
int8 getPositionIndex(int8 fig, int8 color, int8 number=0);
int8 getFigureNbFromPosIdx(int8 pos_index, int8 color);
int8 not(int8 color);
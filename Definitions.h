#pragma once

#include <vector>
#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <omp.h>

#ifdef TESTS
#define private public
#endif // TESTS

#define K 0 //king
#define H 1 //queen 
#define W 2 //rook
#define S 3 //knight
#define G 4 //bishop
#define Pc 5 //black pawn
#define Pb 6 //white pawn
#define Pca 7 //black pawn attack
#define Pba 8 //white pawn attack
#define E 17 //empty
#define P 5 //default pawn

#define EMPTY E
#define BLACK 0
#define WHITE 1
#define BOARD_SIZE 64
#define NUMBER_OF_POSITIONS 32
#define NUMBER_OF_PIECES 10
#define ROW_SIZE 8
#define RANGED_FAR 0 // if figure is ranged far then RANGED_FAR stay in end of moves vector
#define DESTROYED -1 // when the figure is attacked her position changes on DESTROYED
#define END 3

using int8 = int;
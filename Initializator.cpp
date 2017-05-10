#include "Initializator.h"
#include "Globals.h"

void Initializator::init()
{
	setInitialBoard();
	setMoves();
}

// 0  1  2  3  4  5  6  7 
// 8  9 10 11 12 13 14 15
//16 17 18 19 20 21 22 23
//24 25 26 27 28 29 30 31
//32 33 34 35 36 37 38 39
//40 41 42 43 44 45 46 47
//48 49 50 51 52 53 54 55
//56 57 58 59 60 61 62 63
void Initializator::setInitialBoard() {
	g_baseBoard.board =
	{ W, S, G, H, K, G, S, W,
		Pc,Pc,Pc,Pc,Pc,Pc,Pc,Pc,
		E, E, E, E, E, E, E, E,
		E, E, E, E, E, E, E, E,
		E, E, E, E, E, E, E, E,
		E, E, E, E, E, E, E, E,
		Pb,Pb,Pb,Pb,Pb,Pb,Pb,Pb,
		W, S, G, H, K, G, S, W };
	g_baseBoard.colors =
	{ BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
		BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
		E,		E,	   E,	  E,	 E,		E,	   E,	  E,
		E,		E,	   E,	  E,	 E,		E,	   E,	  E,
		E,		E,	   E,	  E,	 E,		E,	   E,	  E,
		E,		E,	   E,	  E,	 E,		E,	   E,	  E,
		WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
		WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE };
	g_baseBoard.positions =
	{ 4,  3,  0,  7,  1,  6,  2,  5, // position K H W W S S G G
		8,  9,  10, 11, 12, 13, 14, 15,//			PbPbPbPbPbPbPbPb
		60, 59, 56, 63, 57, 62, 58, 61,//			K H W W S S G G
		48, 49, 50, 51, 52, 53, 54, 55 };//			PwPwPwPbwbPwPwPw
}

void Initializator::setMoves() {
	// pawns moves
	g_figMoves[Pc].push_back(8);
	g_figMoves[Pb].push_back(-8);
	g_figMoves[Pca].push_back(7);
	g_figMoves[Pca].push_back(9);
	g_figMoves[Pba].push_back(-7);
	g_figMoves[Pba].push_back(-9);
	//King moves
	g_figMoves[K].push_back(-8);
	g_figMoves[K].push_back(-7);
	g_figMoves[K].push_back(1);
	g_figMoves[K].push_back(9);
	g_figMoves[K].push_back(8);
	g_figMoves[K].push_back(7);
	g_figMoves[K].push_back(-1);
	g_figMoves[K].push_back(-9);
	//Queen moves
	g_figMoves[H].push_back(-8);
	g_figMoves[H].push_back(-7);
	g_figMoves[H].push_back(1);
	g_figMoves[H].push_back(9);
	g_figMoves[H].push_back(8);
	g_figMoves[H].push_back(7);
	g_figMoves[H].push_back(-1);
	g_figMoves[H].push_back(-9);
	g_figMoves[H].push_back(RANGED_FAR);
	//Rook moves
	g_figMoves[W].push_back(-8);
	g_figMoves[W].push_back(1);
	g_figMoves[W].push_back(8);
	g_figMoves[W].push_back(-1);
	g_figMoves[W].push_back(RANGED_FAR);
	//Bishop moves
	g_figMoves[G].push_back(-7);
	g_figMoves[G].push_back(9);
	g_figMoves[G].push_back(7);
	g_figMoves[G].push_back(-9);
	g_figMoves[G].push_back(RANGED_FAR);
	//Knight moves
	g_figMoves[S].push_back(-15);
	g_figMoves[S].push_back(-6);
	g_figMoves[S].push_back(10);
	g_figMoves[S].push_back(17);
	g_figMoves[S].push_back(15);
	g_figMoves[S].push_back(6);
	g_figMoves[S].push_back(-10);
	g_figMoves[S].push_back(-17);
}



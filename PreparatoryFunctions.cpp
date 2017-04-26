#include "Definitions.h"
#include "Functions.h"
#include <string>

void setInitialBoard(){
	base_board.board = 
		   { W, S, G, H, K, G, S, W,
			 Pc,Pc,Pc,Pc,Pc,Pc,Pc,Pc,
			 E, E, E, E, E, E, E, E,
			 E, E, E, E, E, E, E, E,
			 E, E, E, E, E, E, E, E,
			 E, E, E, E, E, E, E, E,
			 Pb,Pb,Pb,Pb,Pb,Pb,Pb,Pb,
			 W, S, G, H, K, G, S, W };
	base_board.colors =
		   { BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
			 BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
			 E,		E,	   E,	  E,	 E,		E,	   E,	  E,
			 E,		E,	   E,	  E,	 E,		E,	   E,	  E,
			 E,		E,	   E,	  E,	 E,		E,	   E,	  E,
			 E,		E,	   E,	  E,	 E,		E,	   E,	  E,
			 WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
			 WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE };
	base_board.positions = 
		   { 4,  3,  0,  7,  1,  6,  2,  5, // position K H W W S S G G
		     8,  9,  10, 11, 12, 13, 14, 15,//			PbPbPbPbPbPbPbPb
			 60, 59, 56, 63, 57, 62, 58, 61,//			K H W W S S G G
			 48, 49, 50, 51, 52, 53, 54, 55};//			PwPwPwPbwbPwPwPw
}

// 0  1  2  3  4  5  6  7 
// 8  9 10 11 12 13 14 15
//16 17 18 19 20 21 22 23
//24 25 26 27 28 29 30 31
//32 33 34 35 36 37 38 39
//40 41 42 43 44 45 46 47
//48 49 50 51 52 53 54 55
//56 57 58 59 60 61 62 63
void setMoves(){
	// pawns moves
	moves[Pc].push_back(8);
	moves[Pb].push_back(-8);
	moves[Pca].push_back(7);
	moves[Pca].push_back(9);
	moves[Pba].push_back(-7);
	moves[Pba].push_back(-9);
	//King moves
	moves[K].push_back(-8);
	moves[K].push_back(-7);
	moves[K].push_back(1);
	moves[K].push_back(9);
	moves[K].push_back(8);
	moves[K].push_back(7);
	moves[K].push_back(-1);
	moves[K].push_back(-9);
	//Queen moves
	moves[H].push_back(-8);
	moves[H].push_back(-7);
	moves[H].push_back(1);
	moves[H].push_back(9);
	moves[H].push_back(8);
	moves[H].push_back(7);
	moves[H].push_back(-1);
	moves[H].push_back(-9);
	moves[H].push_back(RANGED_FAR);
	//Rook moves
	moves[W].push_back(-8);
	moves[W].push_back(1);
	moves[W].push_back(8);
	moves[W].push_back(-1);
	moves[W].push_back(RANGED_FAR);
	//Bishop moves
	moves[G].push_back(-7);
	moves[G].push_back(9);
	moves[G].push_back(7);
	moves[G].push_back(-9);
	moves[G].push_back(RANGED_FAR);
	//Knight moves
	moves[S].push_back(-15);
	moves[S].push_back(-6);
	moves[S].push_back(10);
	moves[S].push_back(17);
	moves[S].push_back(15);
	moves[S].push_back(6);
	moves[S].push_back(-10);
	moves[S].push_back(-17);
}

string numToSign(int8 a, int8 color){
	switch(a){
	case Pc:
		return "Pc";
	case Pb:
		return "Pb";
	case K:
		if (color == BLACK) return "Kc";
		else return "Kb";
	case H:
		if (color == BLACK) return "Hc";
		else return "Hb";
	case W:
		if (color == BLACK) return "Wc";
		else return "Wb";
	case S:
		if (color == BLACK) return "Sc";
		else return "Sb";
	case G:
		if (color == BLACK) return "Gc";
		else return "Gb";
	}
	return "  ";
}

void printBoard(){
	for (int i = 0; i < 25; i++)
		cout << "#";
	cout << endl;
	for (int i = 0; i < 8; i++){
		cout << '#';
		for (int j = 0; j < 7; j++)
			cout << numToSign(base_board.board[i * 8 + j], base_board.colors[i * 8 + j]) << "|";
		cout << numToSign(base_board.board[i * 8 + 7], base_board.colors[i * 8 + 7]) << '#' << endl;
		if (i < 7){
			cout << "#-----------------------#" << endl;
		}
	}
	for (int i = 0; i < 25; i++)
		cout << "#";
	cout << endl;
}

void printBoard(const Board &chess_board){
	for (int i = 0; i < 25; i++)
		cout << "#";
	cout << endl;
	for (int i = 0; i < 8; i++){
		cout << '#';
		for (int j = 0; j < 7; j++)
			cout << numToSign(chess_board.board[i * 8 + j], chess_board.colors[i * 8 + j]) << "|";
		cout << numToSign(chess_board.board[i * 8 + 7], chess_board.colors[i * 8 + 7]) << '#' << endl;
		if (i < 7){
			cout << "#-----------------------#" << endl;
		}
	}
	for (int i = 0; i < 25; i++)
		cout << "#";
	cout << endl;
}

int8 getPositionIndex(int8 fig, int8 color, int8 number){
	//first pos for:	   K  H  W  S  G  Pc Pb 
    assert(fig >= 0 && fig < 7);
	int8 index_maping[] = { 0, 1, 2, 4, 6, 8, 8 };
    int8 max_number[] =   { 0, 0, 1, 1, 1, 7, 7 };
    number = min(number, max_number[fig]);
    return index_maping[fig] + number + 16 * color;
}

int8 getFigureNbFromPosIdx(int8 pos_index, int8 color){
	int8 index_maping[] = { 0, 1, 2, 2, 3, 3, 4, 4};
	pos_index -= color * 16;
	if (pos_index < 8)
		return index_maping[pos_index];
	return 5 + color;
}

int8 not(int8 color){
    return (color ^ 1);
}

#pragma once
#include "Definitions.h"
#include "Board.h"

class GuiModule {
public:
	static void printBoard();
	static void printBoard(const Board &chessboard);
	static void printWelcomeInfo();
private:
	static std::string numToSign(int8 a, int8 color);
};



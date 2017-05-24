#pragma once
#include <memory>
#include <iostream>
#include "Definitions.h"
#include "Board.h"

class GuiModule {
public:
    static void printBoard();
    static void printBoard(const Board &chessboard, std::ostream &output);
    static void printWelcomeInfo(std::ostream &output);
private:
    static std::string numToSign(int a, int color);
};



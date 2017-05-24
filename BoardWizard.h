#pragma once
#include <array>
#include "Board.h"
namespace BoardWizard {

enum Figure {
    Rwl, Kwl, Bwl, Qnw, Kgw, Bwr, Kwr, Rwr,
    Pw1, Pw2, Pw3, Pw4, Pw5, Pw6, Pw7, Pw8,
    Rbl, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
    Pb1, Pb2, Pb3, Pb4, Pb5, Pb6, Pb7, Pb8, Emp
};

class Wizard
{
public:
    // desc ex. Bwr: B- Bishop w- white r- right, Qnw- Queen white, Pb7- black pawn number 7
    struct FigureData {
        int boardSym;
        int color;
        int number;
        FigureData(int boardSym, int color, int number)
            :boardSym(boardSym), color(color), number(number) {}
    };

    static Board create(const std::array<Figure, BOARD_SIZE>& borad);
    static FigureData getFigureCommonData(Figure figure);
    static int getHash(Board board);
private:
    void fillCommonBoard(Figure figure, int boardIdx);

    static Wizard s_operatingWizard;
    Board m_operatingBoard;
};

}
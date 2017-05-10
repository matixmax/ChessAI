
#include "BoardWizard.h"
#include "FigInfo.h"

using namespace std;

namespace BoardWizard
{
Wizard Wizard::s_operatingWizard = Wizard();

Board Wizard::create(const array<Figure, BOARD_SIZE>& board)
{
    s_operatingWizard.m_operatingBoard = Board::createEmptyBoard();
    for (size_t idx = 0; idx < board.size(); idx++) {
        s_operatingWizard.fillCommonBoard(board[idx], idx);
    }
    return s_operatingWizard.m_operatingBoard;
}

Wizard::FigureData Wizard::getFigureCommonData(Figure figure)
{
    switch (figure)
    {
    case Rwl:
        return FigureData(W, WHITE, 0);
    case Kwl:
        return FigureData(S, WHITE, 0);
    case Bwl:
        return FigureData(G, WHITE, 0);
    case Qnw:
        return FigureData(H, WHITE, 0);
    case Kgw:
        return FigureData(K, WHITE, 0);
    case Bwr:
        return FigureData(G, WHITE, 1);
    case Kwr:
        return FigureData(S, WHITE, 1);
    case Rwr:
        return FigureData(W, WHITE, 1);
    case Pw1:
        return FigureData(Pb, WHITE, 0);
    case Pw2:
        return FigureData(Pb, WHITE, 1);
    case Pw3:
        return FigureData(Pb, WHITE, 2);
    case Pw4:
        return FigureData(Pb, WHITE, 3);
    case Pw5:
        return FigureData(Pb, WHITE, 4);
    case Pw6:
        return FigureData(Pb, WHITE, 5);
    case Pw7:
        return FigureData(Pb, WHITE, 6);
    case Pw8:
        return FigureData(Pb, WHITE, 7);
    case Rbl:
        return FigureData(W, BLACK, 0);
    case Kbl:
        return FigureData(S, BLACK, 0);
    case Bbl:
        return FigureData(G, BLACK, 0);
    case Qnb:
        return FigureData(H, BLACK, 0);
    case Kgb:
        return FigureData(K, BLACK, 0);
    case Bbr:
        return FigureData(G, BLACK, 1);
    case Kbr:
        return FigureData(S, BLACK, 1);
    case Rbr:
        return FigureData(W, BLACK, 1);
    case Pb1:
        return FigureData(Pc, BLACK, 0);
    case Pb2:
        return FigureData(Pc, BLACK, 1);
    case Pb3:
        return FigureData(Pc, BLACK, 2);
    case Pb4:
        return FigureData(Pc, BLACK, 3);
    case Pb5:
        return FigureData(Pc, BLACK, 4);
    case Pb6:
        return FigureData(Pc, BLACK, 5);
    case Pb7:
        return FigureData(Pc, BLACK, 6);
    case Pb8:
        return FigureData(Pc, BLACK, 7);
    case Emp:
        return FigureData(E, E, 0);
    default:
        throw runtime_error("unknown Figure with number" + figure);
    }
}

void Wizard::fillCommonBoard(Figure figure, int boardIdx)
{
    FigureData figData = getFigureCommonData(figure);
    m_operatingBoard.board[boardIdx] = figData.boardSym;
    m_operatingBoard.colors[boardIdx] = figData.color;
    if (figure != Emp) {
        int positionIndex = FigInfo::getPosIndex(figData.boardSym, figData.color, figData.number);
        m_operatingBoard.positions[positionIndex] = boardIdx;
    }
}

}

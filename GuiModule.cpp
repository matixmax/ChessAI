
#include "Definitions.h"
#include "GuiModule.h"
#include <string>
#include "Globals.h"

using namespace std;

void GuiModule::printWelcomeInfo()
{
    cout << "Welocme player" << endl;
    cout << "To show settings write -1 -1 as your move" << endl;
}

string GuiModule::numToSign(int a, int color){
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

void GuiModule::printBoard(){
    printBoard( g_baseBoard );
}

void GuiModule::printBoard(const Board &chess_board){
    vector<char> leftLabels = { '1','2','3','4','5','6','7','8' };
    vector<char> downLabels = { 'A','B','C','D','E','F','G','H' };
    for (int i = 0; i < 27; i++)
        cout << "#";
    cout << endl;
    for (int i = 0; i < 8; i++) {
        cout << ' ' << leftLabels[i] << '#';
        for (int j = 0; j < 7; j++)
            cout << numToSign(chess_board.board[i * 8 + j], chess_board.colors[i * 8 + j]) << "|";
        cout << numToSign(chess_board.board[i * 8 + 7], chess_board.colors[i * 8 + 7]) << '#' << endl;
        if (i < 7){
            cout << "--#-----------------------#" << endl;
        }
    }
    for (int i = 0; i < 27; i++)
        cout << "#";
    cout << endl << "###";
    for (int i = 0; i < 8; i++)
        cout <<" "<<downLabels[i]<< "|";
    cout << endl;
}


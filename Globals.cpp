
#include "Globals.h"

Board g_baseBoard;
std::vector<Special*> g_specials;
std::vector<int> *g_figMoves = new std::vector<int>[10];
const int c_unknownPosition = -404;
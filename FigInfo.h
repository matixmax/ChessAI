#pragma once
#include "Definitions.h"

class FigInfo {
public:
    static int getPosIndex(int fig, int color, int number = 0);
    static int getFigNumber(int pos_index, int color);
    static int not(int color);
};
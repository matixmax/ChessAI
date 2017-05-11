#pragma once

#include "Definitions.h"

class MathLogicStore {
public:
    static int getValueOfPawns(
        int betterSidePawnsSum, std::pair<int, int> &material);
    static int getValueOfMaterial(
        int betterSidePawnsMaterial, int materialAdvantage, std::pair<int, int> &material);
};
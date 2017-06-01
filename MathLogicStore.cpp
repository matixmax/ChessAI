#include "MathLogicStore.h"

int MathLogicStore::getValueOfPawns(int betterSidePawnsSum, std::pair<int, int> &material)
{
    return betterSidePawnsSum * 30000 / ((betterSidePawnsSum + 1) * (material.first + material.second));
}

int MathLogicStore::getValueOfMaterial(int betterSidePawnsMaterial, int materialAdvantage, std::pair<int, int> &material)
{
    //return betterSidePawnsMaterial + (materialAdvantage * 590 * 2 / (material.first + material.second));
    return betterSidePawnsMaterial + materialAdvantage;
}


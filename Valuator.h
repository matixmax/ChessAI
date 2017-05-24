#pragma once

#include "Definitions.h"
#include "Board.h"

namespace Valuation {

enum GameState {
    DEBUT,
    MID_GAME,
    EARLY_ENDING,
    ENDING,
    MATTING,
};

class Valuator {
public:
    static Valuator& i();
    GameState getGamePhase(const Board & chessboard);
    int materialValuation(const Board & chessboard, int color);
    //int pawnsPositionalValue(const Board & chessboard, int color, int phase);//
    //int rooksPositionalValue(const Board & chessboard, int color, int phase);
    //int queenPositionalValue(const Board & chessboard, int color, int phase);
    //int kingPositionalValue(const Board & chessboard, int color, int phase);
    int mattingPositionalValue(const Board & chessboard, int color);
    int knightsPositionalValue(const Board & chessboard, int color);
    int bishopsPositionalValue(const Board & chessboard, int color, int phase);

protected:
    Valuator();

    //int safetyKingPossitionalValue(const Board & chessboard, int color);
    //int wallOfPawnsValuation(const Board & chessboard, int color, int *kings_pos);
    //int connectedRooksAndMobilityValue(const Board & chessboard, int* rooks_pos, int* rooks_color);
    //int rookInOpenLineValue(const Board & chessboard, int* rooks_pos, int* rooks_color);
    static int proximityToTheCenter(int pos);
    static int proximityOfFields(int pos1, int pos2);
    //bool izolatedPawn(const Board & chessboard, int color, int pos);
    //bool mirroredPawn(const Board & chessboard, int color, int pos);
    bool matCondition(const Board &chessboard, int color);
    int getFiguresInStartPos(const Board & chessboard);
    int countSumOfPawns(const Board & chessboard, int color);
    std::pair<int, int> countMaterial(const Board & chessboard);

    std::vector<int> m_figValues;
};

}
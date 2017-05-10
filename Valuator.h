#pragma once

#include "Definitions.h"
#include "GuiModule.h"
#include "Board.h"

class Valuator {
public:
	enum GameState {
		DEBUT,
		MID_GAME,
		EARLY_ENDING,
		ENDING,
		MATTING,
	};
	static Valuator& i();
	int checkGamePhase(const Board & chessboard);
	int materialValuation(const Board & chessboard, int8 color);
	int pawnsPositionalValue(const Board & chessboard, int8 color, int phase);
	int rooksPositionalValue(const Board & chessboard, int8 color, int phase);
	int queenPositionalValue(const Board & chessboard, int8 color, int phase);
	int kingPositionalValue(const Board & chessboard, int8 color, int phase);
	int mattingPositionalValue(const Board & chessboard, int8 color);
	int knightsPositionalValue(const Board & chessboard, int8 color);
	int bishopsPositionalValue(const Board & chessboard, int8 color, int phase);
	
private:
	Valuator();

	int safetyKingPossitionalValue(const Board & chessboard, int8 color);
	int wallOfPawnsValuation(const Board & chessboard, int8 color, int8 *kings_pos);
	int connectedRooksAndMobilityValue(const Board & chessboard, int8* rooks_pos, int8* rooks_color);
	int rookInOpenLineValue(const Board & chessboard, int8* rooks_pos, int8* rooks_color);
	int proximityToTheCenter(int8 pos);
	int proximityOfFields(int8 pos1, int8 pos2);
	bool izolatedPawn(const Board & chessboard, int8 color, int8 pos);
	bool mirroredPawn(const Board & chessboard, int8 color, int8 pos);
	bool matCondition(const Board &chessboard, int8 color);
	int getFiguresInStartPos(const Board & chessboard);
	int countSumOfPawns(const Board & chessboard, int8 color);
	std::pair<int, int> countMaterial(const Board & chessboard);

	std::vector<int> m_figValues;
};
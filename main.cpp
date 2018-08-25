
#include "Definitions.h"
#include "Board.h"
#include "GuiModule.h"
#include "Generator.h"
#include "Valuator.h"
#include "Engine.h"
#include "SpecialsManager.h"
#include "Statistics.h"
#include "Globals.h"
#include "Initializator.h"
#include "GameModule.h"
#include <fstream>
using namespace std;

void turnament() {
	GameModule::i().startGame(BLACK);
	GameModule::i().onGameAITurnament();
}

void test(int number) {
	GameModule::i().startTestMode(BLACK, number);
	GameModule::i().onGamePlayWithPlayer();
}

int main() {
	//test(3);
	turnament();

#ifdef _DEBUG
	system("pause");
#endif
}

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

int main() {
    GameModule::i().startGame(BLACK);
    //GameModule::i().startTestMode(BLACK, 1);
    GameModule::i().onGameAITurnament();
    //GameModule::i().onGamePlayWithPlayer();
    system("pause");
}
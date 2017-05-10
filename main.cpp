#include "Definitions.h"
#include "Board.h"
#include "GuiModule.h"
#include "Generator.h"
#include "Valuator.h"
#include "Engine.h"
#include "SpecialsManager.h"
#include "Debug.h"
#include "Globals.h"
#include "Initializator.h"
#include "GameModule.h"

using namespace std;

int main() {
	
	GameModule::i().startGame(BLACK);

	GameModule::i().onGamePlay();
	system("pause");
}
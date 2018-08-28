from TestPlanRandomCreator import TestPlanRandomCreator
from testPreparator import TestPreparator
from turnamentPreparator import TurnamentPreparator
import shutil
import os
import sys

def generateRandomBase(numberOfTest):
    blackMinRanges = [1,1,1,1,1,1]
    whiteMinRanges = [1,1,1,1,1,1]
    blackMaxRanges = [10,10,10,10,10,10]
    whiteMaxRanges = [10,10,10,10,10,10]    

    testCreator = TestPlanRandomCreator(blackMinRanges, blackMaxRanges, whiteMinRanges, whiteMaxRanges)
    testCreator.createRandomTests(numberOfTests)
    if os.path.exists("testPropertiesChangePlan.txt"):
        os.remove("testPropertiesChangePlan.txt")
    os.rename("testPropertiesChangePlan_out.txt", "testPropertiesChangePlan.txt")

def generateRandomOpposites(numberOfTest, winnerProps, onBlackSide):
    blackMinRanges = [1,1,1,1,1,1]
    whiteMinRanges = [1,1,1,1,1,1]
    blackMaxRanges = [10,10,10,10,10,10]
    whiteMaxRanges = [10,10,10,10,10,10]    

    testCreator = TestPlanRandomCreator(blackMinRanges, blackMaxRanges, whiteMinRanges, whiteMaxRanges)
    testCreator.createOnseSideRandomTests(numberOfTest, winnerProps, onBlackSide)
    if os.path.exists("testPropertiesChangePlan.txt"):
        os.remove("testPropertiesChangePlan.txt")
    os.rename("testPropertiesChangePlan_out.txt", "testPropertiesChangePlan.txt")

def fileContainsTests(filePath):
    with open(filePath) as file:
        return len(file.readlines()) >= 3

def playTest(numberOfTests):
    generateRandomBase(numberOfTests)
    TestPreparator().makeTests()

    turnamentPreparator = TurnamentPreparator("Tests")
    turnamentPreparator.createTestFromWinners()
    level = 0
    while(fileContainsTests("winnersPropertiesChangePlan_out.txt")):
        shutil.copytree("Tests", "Tests_level_" + str(level))
        os.remove("testPropertiesChangePlan.txt")
        os.rename("winnersPropertiesChangePlan_out.txt", "testPropertiesChangePlan.txt")
        TestPreparator().makeTests()
        turnamentPreparator.createTestFromWinners()
        level+=1

def oneSideTurnament(numberOfTests, winnerProps, onBlackSide):
    generateRandomOpposites(numberOfTests, winnerProps, onBlackSide)
    TestPreparator().makeTests()
    turnamentPreparator = TurnamentPreparator("Tests")
    winners = turnamentPreparator.extractWinnersByWinnerName()
    lose = 0
    win = 0
    if onBlackSide:
        for winner in winners:
            if winner == "BLACK WIN":
                win += 1
            if winner == "WHITE WIN":
                lose += 1
    else:
        for winner in winners:
            if winner == "BLACK WIN":
                lose += 1
            if winner == "WHITE WIN":
                win += 1
    return lose, win

def finalOneSideTest(numberOfTests, winnerProps):
    loses, wins = oneSideTurnament(numberOfTests, winnerProps, False)
    losesOnBlack, winsOnBlack = oneSideTurnament(numberOfTests, winnerProps, True)
    
    loses += losesOnBlack
    wins += winsOnBlack

    with open("winnerResult.out", 'w') as winnerFile:
        winnerFile.write("BOT LOSE: {0} \t\tBOT WIN {1}".format(loses, wins))

if __name__ == "__main__":
    
    numberOfTests = 50
    props = TurnamentPreparator.getWinnerProp()
    if props != None:
        finalOneSideTest(numberOfTests, props)
    else:
        print("Nobody win")
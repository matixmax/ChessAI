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
        shutil.copytree("Tests", "Tests_level_"+str(level))
        os.remove("testPropertiesChangePlan.txt")
        os.rename("winnersPropertiesChangePlan_out.txt", "testPropertiesChangePlan.txt")
        TestPreparator().makeTests()
        turnamentPreparator.createTestFromWinners()
        level+=1

if __name__ == "__main__":
    if len(sys.argv) > 1:
        numberOfTests = int(sys.argv[1])
    else:
        numberOfTests = 100

    playTest(numberOfTests)
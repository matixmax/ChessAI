import os
import random
import sys

class TestPlanRandomCreator(object):
    def __init__(self, blackMinRanges, blackMaxRanges, whiteMinRanges, whiteMaxRanges):
        self.blackMinRanges = blackMinRanges
        self.blackMaxRanges = blackMaxRanges
        self.whiteMinRanges = whiteMinRanges
        self.whiteMaxRanges = whiteMaxRanges
        self.remember = []

    def getBlackAndWhite(self):
        black = []
        white = []
        for propIdx in range(0,6):
            black.append(random.randint(self.blackMinRanges[propIdx], self.blackMaxRanges[propIdx]))
            white.append(random.randint(self.whiteMinRanges[propIdx], self.whiteMaxRanges[propIdx]))
        if not (black, white) in self.remember:
            self.remember.append((black, white))
            return black, white
        else:
            return self.getBlackAndWhite()

    def createRandomTests(self, numberOfTests):
        random.seed()
        with open("testPropertiesChangePlan_out.txt", 'w') as file:
            file.write("pawnsPosFact;knightsPosFact;bishopsPosFact;rocksPosFact;queenPosFact;kingsPosFact\n")
            for i in range(0, numberOfTests):
                black, white = self.getBlackAndWhite()
                file.write("\t;".join(str(el) for el in black) + "\n")
                file.write("\t;".join(str(el) for el in white) + "\n")
                file.write("\n")
 
if __name__ == "__main__":   
    blackMinRanges = [1,1,1,1,1,1]
    whiteMinRanges = [1,1,1,1,1,1]
    blackMaxRanges = [10,10,10,10,10,10]
    whiteMaxRanges = [10,10,10,10,10,10]              

    if len(sys.argv) > 1:
        numberOfTests = int(sys.argv[1])
    else:
        numberOfTests = 100

    testCreator = TestPlanRandomCreator(blackMinRanges, blackMaxRanges, whiteMinRanges, whiteMaxRanges)
    testCreator.createRandomTests(numberOfTests)



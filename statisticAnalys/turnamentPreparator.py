import os

class TurnamentPreparator(object):
    def __init__(self, testFolderPath):
        if not os.path.exists(testFolderPath):
            raise Exception("test folder path not exists " + testFolderPath)
        self.__testFolderPath = testFolderPath

    def getFileLines(self,filePath):
        with open(filePath) as file:
            return file.readlines()

    def createTestFromWinners(self):
        winnersProps = self.extractWinners();
        if not len(winnersProps) % 2 == 0:
            winnersProps.pop()
        with open("winnersPropertiesChangePlan_out.txt", 'w') as file:
            file.write("pawnsPosFact;knightsPosFact;bishopsPosFact;rocksPosFact;queenPosFact;kingsPosFact\n")
            for i in range(0, int(len(winnersProps)/2)):
                black = winnersProps[i*2]
                white = winnersProps[i*2+1]
                file.write("\t;".join(str(el) for el in black) + "\n")
                file.write("\t;".join(str(el) for el in white) + "\n")
                file.write("\n")

    def extractWinners(self):
        winnersPropsStore = []
        for testDir in os.listdir(self.__testFolderPath):
            testDirFullPath = os.path.join(self.__testFolderPath, testDir)
            winner = self.getWinner(os.path.join(testDirFullPath, "turnamentOutputFile.out"))
            if winner == "NOBODY WIN":
                continue
            winnerProperties = self.getWinnerProperties(winner, os.path.join(testDirFullPath, "properties.txt") )
            winnersPropsStore.append(winnerProperties)
        return winnersPropsStore

    def getWinner(self, turnamentOutputPath):
        turnamentFilelines = self.getFileLines(turnamentOutputPath)
        if len(turnamentFilelines) == 0:
            raise Exception("turnament file is empty" + turnamentOutputPath)
        winnerString = turnamentFilelines[-1]
        if winnerString in ["BLACK WIN", "WHITE WIN", "NOBODY WIN"]:
            return winnerString
        else:
            return "NOBODY WIN"

    def getWinnerProperties(self, winnerString, propertiesFilePath):
        propertiesFilelines= self.getFileLines(propertiesFilePath)
        winnerProperties =[]
        if winnerString == "BLACK WIN":
            winnerIndex = 1
        else:
            winnerIndex = 2
        for property in propertiesFilelines:
            winnerProperties.append(int(property.split(';')[winnerIndex]))
        return winnerProperties
        
if __name__ == "__main__":   
    testsPath = os.path.abspath("Tests")
    preparator = TurnamentPreparator(testsPath)
    preparator.createTestFromWinners();

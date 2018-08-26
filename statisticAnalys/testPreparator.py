import os
import shutil
import re

class testElement:
    black = []
    white = []

class TestPreparator:
    def __init__(self):
        while(os.path.exists("Tests")):
            print("remove Tests")
            shutil.rmtree("Tests", ignore_errors=True)
        os.makedirs("Tests")
        self.data = self.getLines("testPropertiesChangePlan.txt")
        self.paramNames = self.getLineWithoutWitespaces(self.data[0]).split(';')

    def getLineWithoutWitespaces(self, line):
        return re.sub(r'\s+', '', line)

    def makeTests(self):
        tests = []
        index = 1; 
        while index < len(self.data):
            elem = testElement()
            elem.black = self.getLineWithoutWitespaces(self.data[index]).split(';')
            elem.white = self.getLineWithoutWitespaces(self.data[index+1]).split(';')
            if len(elem.black) != len(self.paramNames) or len(elem.white) != len(self.paramNames):
                print("wrong param number for line " + str(index))
            else:
                tests.append(elem)
            index = index + 3

        testId = 0;
        for test in tests:
            self.makeTest(test, testId)
            testId+=1

    def getLines(self, path):
        with open(path, 'r') as plan_file:
            return plan_file.readlines()

    def setPropertiesForTest(self, propertiesPath, test):
        with open(propertiesPath, 'w') as file:
            for idx in range(0, len(self.paramNames)):
                line = self.paramNames[idx] + ';'
                line += test.black[idx] + ';'
                line += test.white[idx] + '\n'
                file.write(line)
        
    def makeTest(self, test, testId):
        testDirShortPath = os.path.join("Tests", "Test_"+str(testId))
        os.makedirs(testDirShortPath)
        shutil.copy2("GraAI.exe", testDirShortPath)
        self.setPropertiesForTest(os.path.join(testDirShortPath, "properties.txt"), test)
        currentDir = os.path.realpath('.')
        os.chdir(testDirShortPath)
        os.system("GraAI.exe")
        os.chdir(currentDir)

if __name__ == "__main__":
    TestPreparator().makeTests()



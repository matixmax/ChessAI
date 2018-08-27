# _*_ coding:utf-8 _*_
import re
import os
import sys
import matplotlib.pyplot as plt

def readData(filename):
    file = open(filename, 'r')
    data = []
    data = file.readlines()
    file.close()
    return data

def extractProperties(propData):
	properties = {}
	for line in propData:
		lineData = line.split()
		properties[lineData[0]] = (lineData[2], lineData[4])
	return properties

def readProperties(data):#get properties data
	propData = []
	if "Properties:" in data[0]:
		for lineId in range(1,len(data)):
			if "####" in data[lineId]: #end of read prop
				break
			propData.append(data[lineId])
	return extractProperties(propData), lineId

def readBoard(data, beginId):
	board = []
	if "####" in data[beginId]:
		board.append(data[beginId])
		for lineId in range(beginId + 1, len(data)):
			board.append(data[lineId])
			if "####" in data[lineId]: #one line before end of board
				lineId += 1
				board.append(data[lineId])
				lineId += 1 # shift to first line after board
				break
	else:
		print("error in read Board on line{0} :{1}".format(beginId, data[beginId]))
	return board, lineId

def extractStatistic(statData):
	statistics = {}
	for line in statData:
		lineData = line.split()
		name = ''.join(line + "_" for line in lineData[:-1])
		statistics[name] = lineData[-1]
	return statistics

def readStatisctics(data, beginId):#get statistics data
	statData = []
	if "####" in data[beginId - 2]:
		for lineId in range(beginId,len(data)):
			if "####" in data[lineId]: #end of read prop
				break
			statData.append(data[lineId])
	else:
		print("error in read Statistics on line{0} :{1}".format(beginId, data[beginId]))
	return extractStatistic(statData), lineId

def readGameOutput(filename):
	data = readData(filename)
	properties, gamedataBeginId = readProperties(data)
	gameData = []
	while gamedataBeginId < len(data) - 1: #after read all data game begin id move to last line id 
		board, gamedataBeginId = readBoard(data, gamedataBeginId)
		statistics, gamedataBeginId = readStatisctics(data, gamedataBeginId)
		gameData.append((board, statistics))

	return properties, gameData

def getSingleStatistic(name, gameData):
	return [singleTurn[1][name] for singleTurn in gameData]

def getSideSingleStatistic(name, color, gameData):
	result = []
	for singleTurnId in range(len(gameData)):
		if gameData[singleTurnId][1]["color_"] == color:
			result.append(gameData[singleTurnId][1][name])
		else:
			if len(result) > 0:
				result.append(result[singleTurnId-1])
			else:
				result.append(0)
	return result

def phaseVal(phase):
	phases = ["DEBUT","MID_GAME","EARLY_ENDING","ENDING","MATTING"]
	for id in range(len(phases)):
		if phases[id] == phase:
			return id

def getPhaseStatistic(gameData):
	return [phaseVal(singleTurn[1]["phase_"]) for singleTurn in gameData]

def getStatisticData(name, gameData):
	if name == "phase":
		return getPhaseStatistic(gameData)
	elif name in ["material BLACK", "material WHITE", "color"]:
		return getSingleStatistic(name, gameData)
	else:
		print("write color")
		color = input()
		return getSideSingleStatistic(''.join(line + "_" for line in name.split()), color, gameData)


properties, gameData = readGameOutput("turnamentOutputFile.out")
while(True):
    print("write statistic name or exit")
    name = input()
    if name == "exit":
        break
    singleStats = getStatisticData(name, gameData)
    plt.plot(singleStats)
    plt.xlabel(name)
    plt.grid(True)
    numberOfMoves = len(gameData)
    scaleDivider
    plt.xticks([x*2 for x in range(0, 45)])
    plt.show()
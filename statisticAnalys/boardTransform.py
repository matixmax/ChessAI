import re

symbolsDictionary = { "Kc":[1,"Kgb"], 
                      "Kb":[1,"Kgw"], 
                      "Hc":[1,"Qnb"], 
                      "Hb":[1,"Qnw"], 
                      "Wc":[1,"Rbl","Rbr"], 
                      "Wb":[1,"Rwl","Rwr"],
                      "Gc":[1,"Bbl","Bbr"],
                      "Gb":[1,"Bwl","Bwr"],
                      "Sb":[1,"Kwl","Kwr"], 
                      "Sc":[1,"Kbl","Kbr"],
                      "Pc":[1,"Pb1","Pb2","Pb3","Pb4","Pb5","Pb6","Pb7","Pb8"],
                      "Pb":[1,"Pw1","Pw2","Pw3","Pw4","Pw5","Pw6","Pw7","Pw8"],
                      "  ":[1,"Emp"]}

def transform(board):
    resultBoard = []
    for line in board:
        boardRow = re.split("[\|#]", line)[1:-1]
        if len(boardRow) != 8:
            continue
        for symbol in boardRow:
            translatedSymbolFigIndex = symbolsDictionary[symbol][0]
            if symbol != "  ":
                symbolsDictionary[symbol][0] += 1
            print symbolsDictionary[symbol][translatedSymbolFigIndex] + ",",
        print ""
       

stringBoard = [ " 1#  |  |  |  |  |  |  |  #",
                "--#-----------------------#",
                " 2#  |  |  |  |  |  |Pc|  #",
                "--#-----------------------#",
                " 3#  |  |  |  |  |  |Pb|Pc#",
                "--#-----------------------#",
                " 4#  |  |  |  |  |Kc|  |Pb#",
                "--#-----------------------#",
                " 5#  |Pc|  |  |  |  |  |  #",
                "--#-----------------------#",
                " 6#Pc|  |  |  |  |  |  |  #",
                "--#-----------------------#",
                " 7#Wb|  |  |  |  |  |  |  #",
                "--#-----------------------#",
                " 8#  |  |Wc|  |  |  |Kb|  #"]

transform(stringBoard)
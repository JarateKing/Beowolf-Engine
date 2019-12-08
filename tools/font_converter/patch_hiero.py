# hiero can provide lots of good information
# like glyph width and various kerning stuff
# but hiero's atlas is fairly tightly packed
# while this converter doesn't pack them any
# so we need to modify its x & y positioning

import re

tilesX = 16
tilesY = 8
tileSize = 64
tileXoffset = 5

curPos = 0
fntFile = ""

with open("opensans.fnt") as file:
	fntFile = file.read()
	
fntFile = re.sub("height=\d+", "height=64", fntFile)
fntFile = re.sub("yoffset=\d+", "yoffset=0", fntFile)
fntFile = re.sub("scaleW=\d+ scaleH=\d+", "scaleW=" + str(tilesX * tileSize) + " scaleH=" + str(tilesY * tileSize), fntFile)

with open("fontsheet.txt") as charList:
	for char in charList:
		n = int(char)
		
		pattern = "char id=" + str(n) + "\s+x=\d+\s+y=\d+"
		replace = "char id=" + str(n) + " x=" + str(curPos % tilesX * tileSize + tileXoffset) + " y=" + str(curPos // tilesX * tileSize)
		fntFile = re.sub(pattern, replace, fntFile)
		
		curPos = curPos + 1
		
print(fntFile)

# TODO: save modified file

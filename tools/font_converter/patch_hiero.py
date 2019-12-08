# hiero can provide lots of good information
# like glyph width and various kerning stuff
# but hiero's atlas is fairly tightly packed
# while this converter doesn't pack them any
# so we need to modify its x & y positioning

import re

tilesX = 16
tilesY = 8
tileSize = 64

curPos = 0
fntFile = ""

with open("opensans.fnt") as file:
	fntFile = file.read()

with open("fontsheet.txt") as charList:
	for char in charList:
		n = int(char)
		
		pattern = "char id=" + str(n) + "\s+x=\d+\s+y=\d+"
		replace = "char id=" + str(n) + " x=" + str(curPos % tilesX * tileSize) + " y=" + str(curPos // tilesX * tileSize)
		fntFile = re.sub(pattern, replace, fntFile)
		
		# may have to set height to 64 for everything
		# and y offset to 0 along with that
		# TODO: find way that I don't have to
		
		curPos = curPos + 1
		
print(fntFile)

# TODO: save modified file

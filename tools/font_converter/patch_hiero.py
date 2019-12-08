# hiero can provide lots of good information
# like glyph width and various kerning stuff
# but hiero's atlas is fairly tightly packed
# while this converter doesn't pack them any
# so we need to modify its x & y positioning

tilesX = 16
tilesY = 8
tileSize = 64

curPos = 0
with open("fontsheet.txt") as charList:
	for char in charList:
		n = int(char)
		print(n)
		
		# find the line in the file
		# replace the line with new xpos / ypos based off curPos
		
		curPos = curPos + 1
from PIL import Image
import sys
import os

imgFile = Image.open(sys.argv[1])
pixels = imgFile.load()

img = Image.new(imgFile.mode, imgFile.size)
newpixels = img.load()
for i in range(img.size[0]):
	for j in range(img.size[1]):
		nonzeroCount = 0
		for k in range(min(3, 0 if type(pixels[i, j]) is int else len(pixels[i, j]))):
			if pixels[i, j][k] > 0:
				nonzeroCount = nonzeroCount + 1
		if nonzeroCount < 2:
			newpixels[i, j] = (0, 0, 0, 255)
		else:
			newpixels[i, j] = pixels[i, j]
img.save(sys.argv[1][:-4] + "cropped" + sys.argv[1][-4:])
os.remove(sys.argv[1])
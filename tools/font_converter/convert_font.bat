for /F "tokens=*" %%A in (fontsheet.txt) do (
	bin\msdfgen.exe -font "%1" %%A -o "text_%%A.png" -size 64 64 -scale 1.75 -translate 5 10
	python bin\remove_dead_pixels.py text_%%A.png
)
bin\montage.exe -geometry 64x64+0+0 -tile 16x8 *.png -background black "%1.tga"
del *.png
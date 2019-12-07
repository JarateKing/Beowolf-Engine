for /F "tokens=*" %%A in (fontsheet.txt) do (
	msdfgen.exe -font "%1" %%A -o "text_%%A.png" -size 64 64 -scale 1.8 -translate 1 9
)
montage -geometry 64x64+0+0 -tile 16x8 *.png -background black "%1.tga"
del *.png
for /F "tokens=*" %%A in (fontsheet.txt) do (
	msdfgen.exe -font "%1" %%A -o "text_%%A.png" -size 64 64 -autoframe
)
montage -geometry 64x64+0+0 -tile x8 *.png -background black "%1.tga"
del *.png
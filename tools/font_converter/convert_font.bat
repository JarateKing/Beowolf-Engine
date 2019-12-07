for /F "tokens=*" %%A in (fontsheet.txt) do (
	msdfgen.exe -font "%1" %%A -o "text_%%A.png" -size 64 64 -autoframe
)
montage -geometry 64x64+0+0 *.png "%1.tga"
del *.png
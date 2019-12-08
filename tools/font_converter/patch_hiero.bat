bin\patch_hiero.py "%1" > temp.fnt
del "%1"
move "temp.fnt" "%1"
pause

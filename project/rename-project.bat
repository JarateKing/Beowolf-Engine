@echo off

:: get original name
if not [%1]==[] set oldname=%1
if [%1]==[] set /p oldname="Name of original project: "

:: get new name
if not [%2]==[] set newname=%2
if [%2]==[] set /p newname="New name for project: "

move %oldname% %newname%
cd %newname%
move %oldname% %newname%
move %oldname%.sln %newname%.sln
move %oldname%.vcxproj %newname%.vcxproj
move %oldname%.vcxproj.filters %newname%.vcxproj.filters
move %oldname%.vcxproj.user %newname%.vcxproj.user
cd ..

pause

@echo off

:: get original name
if not [%1]==[] set oldname=%1
if [%1]==[] set /p oldname="Name of original project: "

:: get new name
if not [%2]==[] set newname=%2
if [%2]==[] set /p newname="New name for project: "

:: change folder and file names
move %oldname% %newname%
cd %newname%
move %oldname% %newname%
move %oldname%.sln %newname%.sln
move %oldname%.vcxproj %newname%.vcxproj
move %oldname%.vcxproj.filters %newname%.vcxproj.filters
move %oldname%.vcxproj.user %newname%.vcxproj.user

:: change internal references to project name
call powershell -Command "(gc %newname%.sln) -replace '"%oldname%"', '"%newname%"' | Out-File %newname%.sln"
call powershell -Command "(gc %newname%.sln) -replace '"%oldname%.vcxproj"', '"%newname%.vcxproj"' | Out-File %newname%.sln"
call powershell -Command "(gc %newname%.vcxproj) -replace '<RootNamespace>%oldname%</RootNamespace>', '<RootNamespace>%newname%</RootNamespace>' | Out-File %newname%.vcxproj"
call powershell -Command "(gc %newname%.vcxproj) -replace 'Include=\"%oldname%', 'Include=\"%newname%' | Out-File %newname%.vcxproj"
call powershell -Command "(gc %newname%.vcxproj.filters) -replace 'Include=\"%oldname%', 'Include=\"%newname%' | Out-File %newname%.vcxproj.filters"

:: return to original directory
cd ..

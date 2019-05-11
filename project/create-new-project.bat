@echo off

:: get project name
if not [%1]==[] set projname=%1
if [%1]==[] set /p projname="Name of new project: "

if exist %projname% GOTO ERROR

:: make copy of example folder
move example example-temporary-to-rename
mkdir example
xcopy /E /EXCLUDE:ignored-files.txt example-temporary-to-rename example
call rename-project.bat example %projname%
move example-temporary-to-rename example

:: we have successfully done what we need to
GOTO DONE

:ERROR
echo ERROR
pause
GOTO DONE

:DONE

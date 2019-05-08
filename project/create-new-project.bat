@echo off

:: get project name
set /p projname="Name of new project: "

mkdir %projname%
xcopy /E /EXCLUDE:ignored-files.txt example %projname%

@echo off

:: get project name
set /p projname="Name of new project: "

mkdir %projname%
xcopy /E example %projname%

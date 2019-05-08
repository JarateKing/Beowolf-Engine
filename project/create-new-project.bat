@echo off

:: get project name
set /p projname="Name of new project: "

move example example-temporary-to-rename
mkdir example
xcopy /E /EXCLUDE:ignored-files.txt example-temporary-to-rename example
call rename-project.bat example %projname%
move example-temporary-to-rename example

@echo off
echo What type of Visual Studio are you using (Ex: 'vs2019', 'vs2017')
set /p vsVer=
call vender\bin\premake\premake5.exe %vsVer%
pause
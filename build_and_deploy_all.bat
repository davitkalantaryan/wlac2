::
:: Created on:	Jan. 03 2018
:: Autor:	Davit Kalantaryan
::
:: Purpose:	This script sets the correct platfor for window compilation
::
:: Argumet list:This script expects 1 argumets, that shows which server to start
::
:: devenv wlac-all.sln /rebuild Debug /project wlac-all.vcxproj /projectconfig "Debug|Win32"  /log logWin32.txt
::

@ECHO off

SETLOCAL enableextensions

set  win64Version=8.1
set  win32Version=8.1
set  armVersion=10.0.17763.0

:: calculating current directory
SET  scriptDirectory=%~dp0
set  currentDirectory=%cd%


set  pltSetterExe=%scriptDirectory%util\set_windows_target_platform.exe


cd %scriptDirectory%workspaces\wlac-all_vs


%pltSetterExe% wlac-all.vcxproj %win32Version%
::devenv wlac-all.sln /rebuild "Debug|x86" /project wlac-all.vcxproj
call %scriptDirectory%util\batch_set_windows_target_platform.bat %win32Version%
devenv wlac-all.sln /build "Debug|x86"


%pltSetterExe% wlac-all.vcxproj %armVersion%
::devenv wlac-all.sln /rebuild "Debug|ARM" /project wlac-all.vcxproj
::devenv wlac-all.sln /rebuild "Debug|ARM" /project wlac-all.vcxproj
call %scriptDirectory%util\batch_set_windows_target_platform.bat %armVersion%
devenv wlac-all.sln /build "Debug|ARM"

%pltSetterExe% wlac-all.vcxproj %win64Version%
::devenv wlac-all.sln /rebuild "Debug|x64" /project wlac-all.vcxproj
call %scriptDirectory%util\batch_set_windows_target_platform.bat %win64Version%
devenv wlac-all.sln /build "Debug|x64" 


cd %currentDirectory%

ENDLOCAL

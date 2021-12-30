@echo off
pushd %~dp0\..\
CALL vendor\bin\premake\premake5.exe vs2022
popd

IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)
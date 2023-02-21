@echo off

IF NOT EXIST %~dp0\..\..\build mkdir %~dp0\..\..\build
pushd %~dp0\..\..\build

cl /nologo /std:c17 /FC /Zi /Wall /WX /external:anglebrackets /external:W0 ..\breakout\code\win32_breakout.c /link /WX user32.lib gdi32.lib

popd

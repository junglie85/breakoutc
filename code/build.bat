@echo off

IF NOT EXIST %~dp0\..\..\build mkdir %~dp0\..\..\build
pushd %~dp0\..\..\build

cl /nologo /std:c17 /FC /Zi /Wall /WX /wd4820 /external:anglebrackets /external:W0 ..\breakout\code\win32_breakout.c /link /WX user32.lib gdi32.lib /incremental:no /opt:ref

popd

REM Compiler flags:

REM /nologo  : don't show the logo
REM /std:c17 : use the c17 standard
REM /FC      : use full path to source file in messages
REM /Zi      : generate debug info
REM /Wall    : enable all warnings
REM /WX      : treat warnings as errors
REM /wdXXXX  : disable warning XXXX
REM /external:anglebrackets : treat includes in angle brackets as third party source
REM /external:W0            : disable all warnings in third party code

REM Linker flags:

REM /WX            : treat warnings as errors
REM incremental:no : disable incremental builds
REM opt:ref        : exclude unused functions and data

REM Compiler warnings:

REM C4820 : 'bytes' bytes padding added after construct 'member_name'

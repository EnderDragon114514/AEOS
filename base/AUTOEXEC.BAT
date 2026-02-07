@echo off
ctmouse.exe
:loop
abki.exe
if ERRORLEVEL 1 shutdown s
goto loop

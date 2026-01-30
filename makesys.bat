rem AEOS v5.11.2 build 6937 Automatic Build Script
rem This script can only run in DOS mode
@echo off
cd .\bin
.\bin\tcc.exe /l.\lib /i.\include /e.\src\aeos41.exe .\src\aeos41.cpp .\lib\graphics.lib
copy .\output\aeos41.exe .\output\aeos41.old
del .\soutput\aeos41.exe
copy .\bin\aeos41.exe .\output\aeos41.exe
.\bin\tcc.exe /l.\lib /i.\include /e.\src\bgidm.exe .\src\bgidm.cpp .\lib\graphics.lib
copy .\src\bgidm.exe .\src\bgidm.old
del .\src\bgidm.exe
copy .\bin\bgidm.exe .\src\bgidm.exe
.\bin\tcc.exe /l.\lib /i.\include /e.\src\vga.exe .\src\vga.cpp .\lib\graphics.lib
copy .\src\vga.exe .\src\vga.old
del .\src\vga.exe
copy .\bin\vga.exe .\src\vga.exe
.\bin\tcc.exe /l.\lib /i.\include /e.\src\init.exe .\src\init.cpp
copy .\src\init.exe .\src\init.old
del .\src\init.exe
copy .\bin\init.exe .\src\init.exe
.\bin\tcc.exe /l.\lib /i.\include /e.\src\51kern.exe .\src\51kern.cpp
copy .\src\51kern.exe .\src\51kern.old
del .\src\51kern.exe
copy .\bin\51kern.exe .\src\51kern.exe
rem AEOS v5.11.2 build 7049 patch 3 Automatic Build Script
rem This script can only run in DOS mode
@echo off
cd .\bin
.\bin\tcc.exe /l.\lib /i.\include /e.\src\aeos41.exe .\src\aeos41.cpp .\lib\graphics.lib
copy .\output\aeos41.exe .\output\aeos41.old
del .\output\aeos41.exe
copy .\bin\aeos41.exe .\output\aeos41.exe
.\bin\tcc.exe /l.\lib /i.\include /e.\src\bgidm.exe .\src\bgidm.cpp .\lib\graphics.lib
copy .\output\bgidm.exe .\output\bgidm.old
del .\output\bgidm.exe
copy .\bin\bgidm.exe .\src\bgidm.exe
.\bin\tcc.exe /l.\lib /i.\include /e.\src\vga.exe .\src\vga.cpp .\lib\graphics.lib
copy .\output\vga.exe .\output\vga.old
del .\output\vga.exe
copy .\bin\vga.exe .\src\vga.exe
.\bin\tcc.exe /l.\lib /i.\include /e.\src\init.exe .\src\init.cpp
copy .\output\init.exe .\output\init.old
del .\output\init.exe
copy .\bin\init.exe .\src\init.exe.\BIN\TCC.EXE /mh /L.\LIB /I.\INCLUDE /E.\src\51KERN.EXE .\src\51KERN.CPP .\src\MEMDEF.CPP .\src\MEMDEF.H
pause
copy .\output\51KERN.exe .\output\51KERN.old
del .\output\51KERN.exe
copy .\BIN\51KERN.EXE .\output\51KERN.exe
.\BIN\TCC.EXE /mh /L.\LIB /I.\INCLUDE /E.\src\ABKI.EXE .\src\ABKI.CPP
pause
copy .\output\ABKI.exe .\output\ABKI.old
del .\output\ABKI.exe